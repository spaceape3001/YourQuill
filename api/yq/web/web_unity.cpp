////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "WebHtml.hpp"
#include "WebPage.hpp"
#include "WebTemplate.hpp"
#include "WebVariable.hpp"
//#include "NetWriter.hpp"

#include <yq/collection/EnumMap.hpp>
#include <yq/collection/Map.hpp>
#include <yq/file/FileUtils.hpp>
#include <yq/http/HttpDataStream.hpp>
#include <yq/http/HttpRequest.hpp>
#include <yq/http/HttpResponse.hpp>
#include <yq/log/Logging.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/stream/Text.hpp>
#include <yq/text/Utils.hpp>

#include <tbb/spin_rw_mutex.h>

namespace yq {

    // Conditional read lock
#define LOCK                                                        \
    const auto& _r = webRepo();                                     \
    tbb::spin_rw_mutex::scoped_lock     _lock;                      \
    if(_r.openReg)                                                  \
        _lock.acquire(_r.mutex, false);

    // Unconditional read lock
#define LLOCK                                                       \
    const auto& _r = webRepo();                                     \
    tbb::spin_rw_mutex::scoped_lock     _lock(_r.mutex, false);     \

    // Unconditional write lock
#define WLOCK                                                       \
    auto& _r = webRepo();                                           \
    tbb::spin_rw_mutex::scoped_lock     _lock(_r.mutex, true);

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#if 0
    void NetWriter::title(std::string_view t)
    {
        m_title     = t;
    }
#endif

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    namespace {
        using WebPageMap    = EnumMap<HttpOp, Map<std::string_view, const WebPage*, IgCase>>;
        using WebVarMap     = Map<std::string_view, const WebVariable*, IgCase>;

        static constexpr const char*    szDefaultTemplate = "<HTML><HEAD><TITLE>{{TITLE}}</TITLE></HEAD><BODY><H1>{{TITLE}}</H1>{{BODY}}</BODY></HTML>";
        

        
        struct WebRepo {
            std::vector<const WebPage*>     allPages;
            std::vector<const WebVariable*> allVars;
            WebPageMap                      dirs;
            WebPageMap                      exts;
            WebPageMap                      globs;
            WebPageMap                      pages;
            WebVarMap                       variables;
            Ref<WebTemplate>                htmlTemplate;
            mutable tbb::spin_rw_mutex      mutex;
            volatile bool                   openReg    = true;
            
            WebRepo()
            {
                htmlTemplate    = new WebTemplate;
            }
        };
    
        WebRepo& webRepo() 
        {
            static WebRepo s_ret;
            return s_ret;
        }
        
        Ref<WebTemplate>    htmlTemplate()
        {
            LLOCK
            return _r.htmlTemplate;
        }
    }


    namespace web {

        const std::vector<const WebPage*>&  all_pages()
        {
            return webRepo().allPages;
        }
        
        const std::vector<const WebVariable*>& all_variables()
        {
            return webRepo().allVars;
        }

        const WebPage*  directory(HttpOp m, std::string_view sv)
        {
            LOCK
            return _r.dirs[m].get(sv, nullptr);
        }
        
        const WebPageMap&    directory_map()
        {
            return webRepo().dirs;
        }

        const WebPage*  extension(HttpOp m, std::string_view sv)
        {
            LOCK
            return _r.exts[m].get(sv, nullptr);
        }

        void freeze()
        {
            WLOCK
            _r.openReg = false;
        }
        
        bool frozen()
        {
            LOCK
            return _r.openReg;
        }

        const WebPageMap&    glob_map()
        {
            return webRepo().globs;
        }
            
        const WebPageMap&    extension_map()
        {
            return webRepo().exts;
        }
        
        const WebPage* glob(HttpOp m, std::string_view sv)
        {
            LOCK
            return _r.globs[m].get(sv, nullptr);
        }

        const WebPage* page(HttpOp m, std::string_view sv)
        {
            LOCK
            return _r.pages[m].get(sv, nullptr);
        }

        const WebPageMap&    page_map()
        {
            return webRepo().pages;
        }
        
        // Sets the template (will reload as necessary)
        bool                set_template(const std::filesystem::path& p)
        {
            std::string     data    = file_string(p);
            if(data.empty())    
                return false;
            Ref<WebTemplate>   ht  = new WebTemplate(std::move(data));
            {
                WLOCK
                std::swap(_r.htmlTemplate, ht);
            }
            return true;
        }
        
        // Sets the template to content
        bool                set_template(std::string_view sv)
        {
            if(sv.empty())
                return false;
            Ref<WebTemplate>   ht  = new WebTemplate(sv);
            {
                WLOCK
                std::swap(_r.htmlTemplate, ht);
            }
            return true;
        }

        const WebVariable*   variable(std::string_view k)
        {
            LOCK
            return _r.variables.get(k, nullptr);
        }
    
        const WebVarMap&    variable_map()
        {
            return webRepo().variables;
        }
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    WebHtml::WebHtml(WebContext&ctx, const std::string_view& _title) : m_context(ctx), m_title(_title)
    {
        m_body.reserve(65536);
    }
    
    WebHtml::~WebHtml()
    {
        run_me();
    }

    void    WebHtml::run_me()
    {
        auto temp   = htmlTemplate();
        HttpDataStream  out(m_context.reply.content(ContentType::html));
        for(auto& t : temp->m_bits){
            if(!t.variable){
                out << t.token;
                continue;
            }
            
            if(is_similar(t.token, "body")){
                out << m_body;
            } else if(is_similar(t.token, "title")){
                //out << m_title;
                html_escape_write(out, m_title);
            } else {
                const WebVariable*  v = web::variable(t.token);
                if(v){
                    v -> handle(out, m_context);
                }
            }
        }
    }

    bool WebHtml::write(const char* buf, size_t cb) 
    {
        m_body.append(buf, cb);
        return true;
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    WebPage::WebPage(HttpOps _methods, std::string_view p, const std::source_location& sl) : Meta(p, sl)
    {
        set_option(WEB);
        
        m_methods   = _methods;
        
        WLOCK
        if(_r.openReg){
            _r.allPages.push_back(this);
        }
    }
    
    WebPage::~WebPage()
    {
    }

    bool  WebPage::anonymouse_posting() const
    {
        return static_cast<bool>(flags() & POST_ANON);
    }

    bool  WebPage::local_only() const
    {
        return static_cast<bool>(flags() & LOCAL_ONLY);
    }
    
    bool  WebPage::login_required() const
    {
        return static_cast<bool>(flags() & LOGIN_REQ);
    }
    
    bool  WebPage::no_expansion() const
    {
        return static_cast<bool>(flags() & NO_EXPAND);
    }

    void    WebPage::seal()
    {
        if(flags() & SEALED)
            return ;
            
        if(!(flags() & DISABLE_REG)){
            std::string_view    p   = path();
            if((!p.empty()) && (m_role == Role())){
                switch(p[0]){
                case '/':
                    if(ends(p, "/*")){
                        p   = p.substr(0, p.size()-2);
                        m_role  = Role::Directory;
                    } else if(ends(p, "/**")){
                        p   = p.substr(0, p.size()-3);
                        m_role  = Role::Glob;
                    } else {
                        m_role  = Role::Page;
                    }
                    break;
                case '*':
                    if(starts(p, "*.")){
                        p   = p.substr(2);
                        m_role  = Role::Extension;
                    }
                    break;
                default:
                    break;
                }
            }
            
            WLOCK
            switch(m_role){
            case Role::Directory:
                for(HttpOp m : HttpOp::all_values())
                    if(m_methods.is_set(m))
                        _r.dirs[m][p]     = this;
                break;
            case Role::Extension:
                for(HttpOp m : HttpOp::all_values())
                    if(m_methods.is_set(m))
                        _r.exts[m][p]     = this;
                break;
            case Role::Glob:
                for(HttpOp m : HttpOp::all_values())
                    if(m_methods.is_set(m))
                        _r.globs[m][p]     = this;
                break;
            case Role::Page:
                for(HttpOp m : HttpOp::all_values())
                    if(m_methods.is_set(m))
                        _r.pages[m][p]     = this;
                break;
            default:
                break;
            }
        }
        
        set_option(SEALED);
    }


    //  ----------------------------------------------------------------------------------------------------------------
    WebPage::Writer::Writer(WebPage*p) : Meta::Writer(p), m_page(p)
    {
    }

    WebPage::Writer::Writer(Writer&& mv) : Meta::Writer(std::move(mv)), m_page(mv.m_page)
    {
        mv.m_page = nullptr;
    }
    
    WebPage::Writer& WebPage::Writer::operator=(Writer&& mv)
    {
        if(this != &mv){
            if(m_page)
                m_page -> seal();
            m_page  = mv.m_page;
            mv.m_page   = nullptr;
        }
        return *this;
    }
    
    WebPage::Writer::~Writer()
    {
        if(m_page)
            m_page -> seal();
    }

    WebPage::Writer&  WebPage::Writer::anon_post()
    {
        if(m_page)
            m_page -> set_option(POST_ANON);
        return *this;
    }

    WebPage::Writer&  WebPage::Writer::argument(std::string_view k, std::string_view d) 
    {
        if(m_page)
            m_page -> m_args.push_back( Arg{k, d});
        return *this;
    }
    
    WebPage::Writer&  WebPage::Writer::content(ContentTypes ct)
    {
        if(m_page){
            m_page -> m_content_types   |= ct;
        }
        return *this;
    }

    WebPage::Writer&  WebPage::Writer::description(std::string_view sv)
    {
        if(m_page)
            Meta::Writer::description(sv);
        return *this;
    }
    
    WebPage::Writer&  WebPage::Writer::disable_reg()
    {
        if(m_page)
            m_page -> set_option(DISABLE_REG);
        return *this;
    }
    
    
    WebPage::Writer&  WebPage::Writer::local()
    {
        if(m_page)
            m_page -> set_option(LOCAL_ONLY);
        return *this;
    }

    WebPage::Writer&  WebPage::Writer::login()
    {
        if(m_page)
            m_page -> set_option(LOGIN_REQ);
        return *this;
    }
    
    
    WebPage::Writer&  WebPage::Writer::no_expand()
    {
        if(m_page)
            m_page -> set_option(NO_EXPAND);
        return *this;
    }

    WebPage::Writer&  WebPage::Writer::primary(ContentType ct)
    {
        if(m_page){
            m_page -> m_content_type    = ct;
            m_page -> m_content_types   |= ct;
        }
        return *this;
    }
    
    
    WebPage::Writer& WebPage::Writer::role(Role r)
    {
        if(m_page){
            m_page -> m_role = r;
        }
        return *this;
    }
    
    WebPage::Writer& WebPage::Writer::sub(std::string_view p, const WebPage*w)
    {
        if(w){
            sub(w->methods(), p, w);
        }
        return *this;
    }
    

    WebPage::Writer& WebPage::Writer::sub(HttpOps m, std::string_view p, const WebPage*w)
    {
        if(w && m_page){
            for(HttpOp h : HttpOp::all_values()){
                if(m.is_set(h)){
                    m_page -> set_option(HAS_SUBS);
                    m_page -> m_subs[h][p]  = w;
                }
            }
        }
        return *this;
    }
    

    //  ------------------------------------------------

#if 0

        class WebFileHandler : public WebPage {
        public:
        
            WebFileHandler(HttpOps m, std::string_view p, const std::source_location& sl, const std::filesystem::path&f) : WebPage(m, p, sl), m_file(f) {}

            virtual void handle(WebContext&ctx) const override 
            {
                switch(ctx.request.method()){
                case HttpOp::Get:
                    send_file(m_file, ctx.reply);
                    break;
                case HttpOp::Head:
                    send_file_info(m_file, ctx.reply);
                    break;
                default:
                    throw HttpStatus::MethodNotAllowed;
                }
            }
        
            std::filesystem::path       m_file;
        };


    namespace {
    
        class WebDirHandler : public WebPage {
        public:
            WebDirHandler(HttpOps m, std::string_view p, const std::source_location& sl, const std::filesystem::path& dpath) : WebPage(m, p, sl), m_dir(dpath){}
            
            virtual void handle(const HttpRequest& rq, HttpResponse& rs, std::string_view sl) const override 
            {
                std::filesystem::path       toload =  m_dir / path_sanitize(sl);
                switch(rq.method()){
                case HttpOp::Get:
                    send_file(toload, rs);
                    break;
                case HttpOp::Head:
                    send_file_info(toload, rs);
                    break;
                default:
                    throw HttpStatus::MethodNotAllowed;
                }
            }
            
            std::filesystem::path   m_dir;
        };
        
        class WebPathVecHandler : public WebPage {
        public:
            WebPathVecHandler(HttpOps m, std::string_view p, const std::source_location& sl, const path_vector_t& paths) : WebPage(m, p, sl), m_paths(paths){}
            
            
            std::filesystem::path       resolve(std::string_view sl) const
            {
                
            }
            
            
            virtual void handle(const HttpRequest& rq, HttpResponse& rs, std::string_view sl) const override 
            {
                std::filesystem::
                for(const std::filesystem
            }
            

        };
    }

    WebPage::Writer     reg_web(std::string_view p, const std::filesystem::path& fs, const std::source_location& sl)
    {
        if(std::filesystem::is_directory(fs)){
            return { nullptr }; // { new WebDirHandler( { HttpOp::Get, HttpOp::Head }, p,sl, fs) };
        } else {
            return { new WebFileHandler( { HttpOp::Get, HttpOp::Head }, p,sl, fs) };
        }
    }
#endif


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    WebTag::WebTag(WebHtml& wh, std::string_view t) : m_html(&wh), m_tag(t)
    {
        wh << '<' << t << '>';
    }
    
    WebTag::WebTag(WebTag&&mv) : m_html(mv.m_html), m_tag(mv.m_tag)
    {
        mv.m_html   = nullptr;
    }
    
    WebTag& WebTag::operator=(WebTag&&mv)
    {
        if(&mv != this){
            close();
            m_html          = mv.m_html;
            mv.m_html       = nullptr;
        }
        return *this;
    }
    
    WebTag::~WebTag()
    {
        close();
    }
    
    void    WebTag::close()
    {
        if(m_html){
            (*m_html) << "</" << m_tag << '>';
            m_html  = nullptr;
        }
    }
        
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        WebTemplate::WebTemplate()
        {
            parse(szDefaultTemplate);
        }

        WebTemplate::WebTemplate(std::string&& mv) : m_data(std::move(mv))
        {
            parse(m_data);
        }
        
        WebTemplate::WebTemplate(std::string_view k) : m_data(k)
        {
            parse(m_data);
        }

        void    WebTemplate::parse(std::string_view data)
        {
            size_t n   = data.find_first_of("{{");
            size_t m   = 0;
            
            if(n == std::string_view::npos){        // no variables... gotcha
                m_bits << Token{ data, false };
                return;
            }
            
            while(n < data.size()){
                if(n > m){
                    //  There's some content before "{{", capture it
                    m_bits << Token{ data.substr(m, n-m), false };
                }

                n += 2;
                m   = data.find_first_of("}}", n);
                if(m>=data.size()){ // badly truncated, dropping....
                    n   = data.size();
                    break;
                }

                std::string_view     k = data.substr(n, m-n);
                m_bits << Token{ k, true };
                m_vars << k;
                
                if(m>=data.size())
                    break;
                
                m += 2;
                n   = data.find_first_of("{{", m);
            }
            
            if(m < data.size()){
                    // no more, so push the remainder on
                m_bits << Token{ data.substr(m), false };
            }

                // YES, possible to lose between "{{NAME" and end if there's no "}}", but that's ill-formed
            
        }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


        WebVariable::WebVariable(std::string_view n, const std::source_location& sl) : Meta(n, sl)
        {
            WLOCK
            if(_r.openReg){
                _r.allVars.push_back(this);
                _r.variables[n] = this;
            }
        }
        
        WebVariable::~WebVariable()
        {
        }


    //  ------------------------------------------------

        WebVariable::Writer::Writer(WebVariable*p) : Meta::Writer(p), m_var(p)
        {
        }

        WebVariable::Writer::Writer(Writer&& mv) : Meta::Writer(std::move(mv)), m_var(mv.m_var)
        {
            mv.m_var = nullptr;
        }
        
        WebVariable::Writer& WebVariable::Writer::operator=(Writer&& mv)
        {
            if(this != &mv){
                //if(m_var)
                    //m_var -> seal();
                m_var  = mv.m_var;
                mv.m_var   = nullptr;
            }
            return *this;
        }
        
        WebVariable::Writer::~Writer()
        {
            //if(m_var)
                //m_var -> seal();
        }


        WebVariable::Writer&  WebVariable::Writer::description(std::string_view sv)
        {
            if(m_var)
                Meta::Writer::description(sv);
            return *this;
        }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    namespace {
        const Set<std::string_view, IgCase>&     httpEntities()
        {
            static const Set<std::string_view, IgCase>    ret({
                "&amp;",
                "&lt;",
                "&gt;"
            });
            return ret;
        }
        
        size_t  is_entity(std::string_view text)
        {
            if(text.empty())
                return 0;
            if(text[0] != '&')
                return 0;
            
            const char* sc  = strnchr(text, ';');
            if(!sc)
                return 0;

            const char* amp = strnchr(text.data()+1, text.size()-1, '&');
            if(amp && (amp < sc))
                return 0;

                // size of this snippet
            size_t  sz  = sc-text.data()+1;
            if(text[1] == '#'){
                for(const char* y=text.data()+2; (y<sc); ++y)
                    if(!is_digit(*y))
                        return 0;
                return sz;
            }
            
            static auto & entities = httpEntities();
            if( entities.has(text.substr(0, sz)))
                return sz;
            return 0;
        }
        
        
        
        template <typename Pred>
        void        iter_http(std::string_view text, Pred p)
        {
            if(text.empty())
                return ;
            
            const char*     next    = nullptr;
            for(const char& c : text){
                if(next && (&c<next)){
                    p(c);
                } else {
                    size_t  sc  = is_entity(text.substr(&c-text.data()));
                    if(sc){
                        next    = &c + sc;
                        p(c);
                    } else {
                        switch(c){
                        case '&':
                            p('&');
                            p('a');
                            p('m');
                            p('p');
                            p(';');
                            break;
                        case '<':
                            p('&');
                            p('l');
                            p('t');
                            p(';');
                            break;
                        case '>':
                            p('&');
                            p('g');
                            p('t');
                            p(';');
                            break;
                        case '"':
                            p('&');
                            p('q');
                            p('u');
                            p('o');
                            p('t');
                            break;
                        case '\'':
                            p('&');
                            p('a');
                            p('p');
                            p('o');
                            p('s');
                            break;
                        default:
                            p(c);
                            break;
                        }
                    }
                }
            }
        }
    }
    
    std::string     html_escape(std::string_view sv)
    {
        std::string ret;
        ret.reserve(sv.size());
        iter_http(sv, [&](char c){
            ret += c;
        });
        return ret;
    }

    void            html_escape_write(Stream& s, std::string_view sv)
    {
        iter_http(sv, [&](char ch){
            s << ch;
        });
    }
    
    #if 0
    namespace {
        template <typename Pred>
        void        send_html(HttpResponse& rs, const WebPage*w, Pred pred)
        {
            //if(expand){
            //} else {
                HttpDataStream      stream(rs.content(ContentType::html));
                HtmlWriter          hwrite(stream);
                pred(hwrite);
            //}
        
            //stream::Text    text;
            //HtmlWriter      hwrite(text);
            //pred(hwrite);
            
            //HttpDataStream  out(rs.content(ContentType::html));
            //out << "<HTML>";
            //std::string_view    title   = hwrite.title();
            
            //if(!title.empty()){
                //out << "<HEAD><TITLE>";
                //html_escape_write(out, title);
                //out <<  "</TITLE></HEAD>\n";
            //}
            
            //out << 
            
            
            
            
        }
        
    }




    WebPage::Writer     reg_web(std::string_view, std::function<void(const HttpRequest&, HtmlWriter&)>, const std::source_location& sl);
    WebPage::Writer     reg_web(HttpOps, std::string_view, std::function<void(const HttpRequest&, HtmlWriter&)>, const std::source_location& sl);

    WebPage::Writer     reg_web(std::string_view, std::function<void(const HttpRequest&, HtmlWriter&, std::string_view)>, const std::source_location& sl);
    WebPage::Writer     reg_web(HttpOps, std::string_view, std::function<void(const HttpRequest&, HtmlWriter&, std::string_view)>, const std::source_location& sl);
    #endif

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    namespace {
        struct FReply {
            ContentType     type;
            size_t          size    = 0;
            char            timestamp[256];
            HttpStatus      code;
            bool            success = false;
        };
    
        bool get_fileinfo(const std::filesystem::path& file, FReply& reply)
        {
            if(reply.type == ContentType())
                reply.type = mimeTypeForExt(file.extension().string().substr(1));
        
            struct stat buf;
            if(::stat(file.c_str(), &buf) != 0)
                return false;
                
                //  we don't do zero sized files
            if(!buf.st_size)
                return false;
            reply.size = buf.st_size;
            
            struct tm   ftime;
            gmtime_r(&buf.st_mtim.tv_sec, &ftime);
            strftime(reply.timestamp, sizeof(reply.timestamp), "%a, %d %b %Y %H:%M:%S GMT", &ftime);
            
            return true;
        }
    }

    bool    send_file_info(const std::filesystem::path& file, HttpResponse& rs, ContentType ct)
    {
        FReply reply;
        reply.type  = ct;
        if(get_fileinfo(file, reply)){
            rs.status(HttpStatus::Success);
            rs.header("Date", reply.timestamp);
            rs.content_type(reply.type);
            rs.header("Content-Length", to_string(reply.size));
            return true;
        } else {
            rs.status(HttpStatus::NotFound);
            return false;
        }
    }

    bool    send_file(const std::filesystem::path& file, HttpResponse& rs, ContentType ct)
    {
        FReply reply;
        reply.type  = ct;
        if(!get_fileinfo(file, reply)){
            rs.status(HttpStatus::NotFound);
            return false;
        }


        int fd   = open(file.c_str(), O_RDONLY);
        if(fd == -1){
            rs.status(HttpStatus::NotFound);
            return false;
        }

        std::vector<HttpDataPtr>&   data    = rs.content();
        
        size_t          amt_read    = 0;
        HttpDataPtr     cur     = HttpData::make();
        ssize_t         n = 0;
        while((n = read(fd, cur->freespace(), cur->available())) > 0){
            cur -> advance(n);
            amt_read += n;
            if(cur->full()){
                data.push_back(cur);
                if(amt_read >= reply.size){
                    cur = nullptr;
                    break;
                }
                cur = HttpData::make();
            }
        } 
        close(fd);
        
        if(cur)
            data.push_back(cur);
        if(amt_read < reply.size){
            rs.status(HttpStatus::InternalError);
        } else {
            rs.status(HttpStatus::Success);
            rs.header("Date", reply.timestamp);
            rs.content_type(reply.type);
        }
        return true;
    }
    
    //void    append(NetWriter&, const std::filesystem::path&)
    //{
    //}
}
