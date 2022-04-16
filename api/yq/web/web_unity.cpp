////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "HttpParser.hpp"
#include "TypedBytes.hpp"
#include "JsonAdapter.hpp"
#include "WebAdapters.hpp"
#include "WebContext.hpp"
#include "WebHtml.hpp"
#include "WebPage.hpp"
#include "WebTemplate.hpp"
#include "WebVariable.hpp"
//#include "NetWriter.hpp"

#include <yq/algo/Random.hpp>
#include <yq/c++/Safety.hpp>
#include <yq/collection/EnumMap.hpp>
#include <yq/collection/Map.hpp>
#include <yq/file/FileUtils.hpp>
#include <yq/log/Logging.hpp>
#include <yq/stream/Bytes.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/stream/Text.hpp>
#include <yq/text/Encode64.hpp>
#include <yq/text/Utils.hpp>


#include <asio/write.hpp>
#include <nlohmann/json.hpp>
#include <tbb/spin_rw_mutex.h>
#include <unistd.h>

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
            
                // This *IS* case sensitive!
            //Map<std::string_view, Ref<WebSession>>  sessions;
            
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

        Ref<WebTemplate>    html_template()
        {
            LLOCK
            return _r.htmlTemplate;
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

    StringViewMultiMap     parse_cookie(std::string_view cs)
    {
        StringViewMultiMap ret;
        vsplit(cs, ';', [&](std::string_view s){
            const char* eq  = strnchr(s, '=');
            if(eq){
                std::string_view k   = trimmed(std::string_view(s.data(), eq));
                std::string_view v   = trimmed(std::string_view(eq+1, s.end()));
                ret.insert(k, v);
            } else {
                ret.insert(trimmed(s), std::string_view());
            }
        });
        return ret;
    }
    
    KVView                  parse_header_line(std::string_view s)
    {
        KVView          ret;
        const char* c   = strnchr(s, ':');
        if(c){
            ret.key     = trimmed(std::string_view(s.data(), c));
            ret.value   = trimmed(std::string_view(c+1, s.end()));
        }
        return ret;
    }



    MethodUriVersion        parse_method_uri(std::string_view input)
    {
        const char* z       = nullptr;
        const char* str     = nullptr;
        
        enum Mode {
            Start    = 0,
            Method,
            MSpace,
            Uri,
            USpace,
            Version,
            Done
        };
        
        Mode                mode    = Start;
        MethodUriVersion    ret;

        for(z = input.begin(); z<input.end(); ++z){
            switch(mode){
            case Start:
                if(!is_space(*z)){
                    str     = z;
                    mode    = Method;
                }
                break;
            case Method:
                if(is_space(*z)){
                    ret.method  = std::string_view(str, z);
                    mode    = MSpace;
                }
                break;
            case MSpace:
                if(is_space(*z))
                    break;
                str     = z;
                mode    = Uri;
                break;
            case Uri:
                if(is_space(*z)){
                    ret.uri = std::string_view(str, z);
                    mode    = USpace;
                }
                break;
            case USpace:
                if(is_space(*z))
                    break;
                str = z;
                mode    = Version;
                break;
            case Version:
                if(is_space(*z)){
                    ret.version = std::string_view(str, z);
                    mode        = Done;
                }
                break;
            default:
                break;
            }
        }
        
        switch(mode){
        case Method:
            ret.method  = std::string_view(str, z);
            break;
        case Uri:
            ret.uri = std::string_view(str, z);
            break;
        case Version:
            ret.version  = std::string_view(str, z);
            break;
        default:
            break;
        }
        
        return ret;
    }
    

    StringViewMultiMap parse_parameters(std::string_view sv)
    {
        StringViewMultiMap  ret;
        if(!sv.empty()){
            vsplit(sv, '&', [&](std::string_view b){
                const char* eq  = strnchr(b, '=');
                if(!eq)
                    return;
                ret.insert(std::string_view(b.data(), eq), std::string_view(eq+1, b.end()));
            });
        }
        return ret;
    }

    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    Ref<TypedBytes> TypedBytes::info(const std::filesystem::path& fp)
    {
        Ref<TypedBytes>     ret = new TypedBytes(fp);
        if(!ret -> do_info())
            return {};
        return ret;
    }

    Ref<TypedBytes> TypedBytes::load(const std::filesystem::path& fp)
    {
        Ref<TypedBytes>     ret = new TypedBytes(fp);
        if(!ret -> do_info())
            return {};
        if(!ret -> do_read())
            return {};
        return ret;
    }

    //  ------------------------------------------------

    bool    TypedBytes::do_info()
    {
        status  = HttpStatus(); // reset the status
        auto ext    = file_extension(path.native());
        if(!ext.empty())
            type    = mimeTypeForExt(ext);
        else
            type    = ContentType();
        
        struct stat buf;
        if(::stat(path.c_str(), &buf) != 0){
            status  = HttpStatus::NotFound;
            return false;
        }

        size        = buf.st_size;
        struct tm   ftime;
        gmtime_r(&buf.st_mtim.tv_sec, &ftime);
        strftime(modified, sizeof(modified), "%a, %d %b %Y %H:%M:%S GMT", &ftime);
        status = HttpStatus::Success;
        return true;
    }

    
    bool    TypedBytes::do_read()
    {
        if(path.empty() || !size){
            status  = HttpStatus::NotFound;
            return false;
        }
        
            // reset for read
        data    = std::make_shared<ByteArray>();
        status  = HttpStatus(); 
        
        int fd   = open(path.c_str(), O_RDONLY);
        if(fd == -1){
            status  = HttpStatus::NotFound;
            return false;
        }
        
        auto sc = safety([fd]() { close(fd); }); // auto-close if necessary
        
        
        ByteArray&ba    = *data;
        ba.resize(size);
        
        size_t          amt_read    = 0;
        
        while(amt_read < size){
            size_t      n   = read(fd, ba.data()+amt_read, size-amt_read);
            if(n <= 0)
                break;
            amt_read += n;
        }
        
        if(amt_read < size){
            status = HttpStatus::InternalError;
        } else {
            status = HttpStatus::Success;
        }
        return true;
    }
    
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        WebAutoClose::WebAutoClose(WebHtml&wh, std::string_view sv) : 
            m_html(&wh)
        {
            m_close = [sv](WebHtml&h) {
                h << sv;
            };
        }
        
        WebAutoClose::WebAutoClose(WebHtml& wh, CloseHandler onClose) : m_html(&wh), m_close(onClose)
        {
        }
        
        WebAutoClose::WebAutoClose(WebAutoClose&&mv) : m_html(mv.m_html), m_close(std::move(mv.m_close))
        {
            mv.m_html   = nullptr;
        }
        
        WebAutoClose& WebAutoClose::operator=(WebAutoClose&&mv)
        {
            if(&mv != this){
                close();
                std::swap(m_html, mv.m_html);
                m_close         = std::move(mv.m_close);
            }
            return *this;
        }
        
        WebAutoClose::~WebAutoClose()
        {
            close();
        }
        
        void    WebAutoClose::close()
        {
            if(m_html){
                if(m_close)
                    m_close(*m_html);
                m_html  = nullptr;
            }
        }



    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        WebContext::WebContext(asio::io_context& _io_ctx) :io_ctx(_io_ctx)
        {
        }
        
        WebContext::~WebContext()
        {
        }
        
        nlohmann::json              WebContext::decode_json() const
        {
            if(rx_body.empty())
                return nlohmann::json();
            return nlohmann::json::parse(rx_body.begin(), rx_body.end());
        }


        //! Decodes post parameters
        StringViewMultiMap          WebContext::decode_post() const
        {
            if((rx_content_type != ContentType()) && (rx_content_type != ContentType::form))
                return StringViewMultiMap();
            return parse_parameters(rx_body);
        }

        StringViewMultiMap          WebContext::decode_query() const
        {
            return parse_parameters(url.query);
        }

        std::string         WebContext::find_query(std::string_view k) const
        {
            return vsplit(url.query, '&', [&](std::string_view b) -> std::string {
                const char* eq  = strnchr(b, '=');
                if(!eq)
                    return std::string();
                if(!is_similar(k, std::string_view(b.data(), eq)))
                    return std::string();
                return web_decode(std::string_view(eq+1, b.end()));
            });
        }


        std::string         WebContext::find_query(std::initializer_list<std::string_view> keys) const
        {
            return vsplit(url.query, '&', [&](std::string_view b) -> std::string {
                const char* eq  = strnchr(b, '=');
                if(!eq)
                    return std::string();
                if(!is_in(std::string_view(b.data(), eq), keys))
                    return std::string();
                return web_decode(std::string_view(eq+1, b.end()));
            });
        }

        void    WebContext::redirect(std::string_view sv, bool permanent)
        {
            if(status != HttpStatus()){
                if(!isRedirect(status)){
                    status    = HttpStatus::InternalError;
                    return ;
                }
            } else {
                status = permanent ? HttpStatus::MovedPermanently : HttpStatus::TemporaryRedirect;
            }
            
            tx_header("Location", sv);
        }




    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        namespace {
            ByteArray&      byteArrayFor(WebContext& ctx)
            {
                if(!ctx.tx_content)
                    ctx.tx_content  = std::make_shared<ByteArray>();
                return *(ctx.tx_content);
            }
        }

        WebHtml::WebHtml(WebContext&ctx, const std::string_view& _title) : m_context(ctx)
        {
            ctx.var_body.reserve(65536);
            if(_title.empty()){
                ctx.var_title.reserve(512);
            } else {
                ctx.var_title       = _title;
            }
        }
        
        WebHtml::~WebHtml()
        {
            run_me();
        }

        void    WebHtml::run_me()
        {
            auto& ba = byteArrayFor(m_context);
            ba.reserve(8192+m_context.var_body.size());   // an estimate....
            auto temp   = web::html_template();
            m_context.tx_content_type   = ContentType::html;
            stream::Bytes   out(ba);
            temp -> execute(out, m_context);
        }

        void WebHtml::title(const std::string_view& _title)
        {
            m_context.var_title     = _title;
        }

        bool WebHtml::write(const char* buf, size_t cb) 
        {
            switch(m_target){
            case BODY:
                m_context.var_body.append(buf, cb);
                break;
            case TITLE:
                m_context.var_title.append(buf, cb);
                break;
            }
            return true;
        }
        
        
        //  ------------------------------

        WebAutoClose              WebHtml::b()
        {
            *this << "<b>"sv;
            return WebAutoClose(*this, "</b>"sv);
        }
        
        WebAutoClose              WebHtml::bold()
        {
            *this << "<b>"sv;
            return WebAutoClose(*this, "</b>"sv);
        }
        
        WebAutoClose              WebHtml::bullets()
        {
            *this << "<ul>"sv;
            return WebAutoClose(*this, "</ul>"sv);
        }
        
        WebAutoClose              WebHtml::h1()
        {
            *this << "<h1>"sv;
            return WebAutoClose(*this, "</h1>"sv);
        }
        
        void                WebHtml::h1(std::string_view s)
        {
            auto wt = h1();
            html_escape_write(*this, s);
        }
        
        WebAutoClose              WebHtml::h2()
        {
            *this << "<h2>"sv;
            return WebAutoClose(*this, "</h2>"sv);
        }
        
        void                WebHtml::h2(std::string_view s)
        {
            auto wt = h2();
            html_escape_write(*this, s);
        }
        
        WebAutoClose              WebHtml::h3()
        {
            *this << "<h3>"sv;
            return WebAutoClose(*this, "</h3>"sv);
        }

        void                WebHtml::h3(std::string_view s)
        {
            auto wt = h3();
            html_escape_write(*this, s);
        }
       
        WebAutoClose              WebHtml::h4()
        {
            *this << "<h4>"sv;
            return WebAutoClose(*this, "</h4>"sv);
        }

        void                WebHtml::h4(std::string_view s)
        {
            auto wt = h4();
            html_escape_write(*this, s);
        }
        
        WebAutoClose              WebHtml::h5()
        {
            *this << "<h5>"sv;
            return WebAutoClose(*this, "</h5>"sv);
        }
        
        void                WebHtml::h5(std::string_view s)
        {
            auto wt = h5();
            html_escape_write(*this, s);
        }
        
        WebAutoClose              WebHtml::h6()
        {
            *this << "<h6>"sv;
            return WebAutoClose(*this, "</h6>"sv);
        }
        
        void                WebHtml::h6(std::string_view s)
        {
            auto wt = h6();
            html_escape_write(*this, s);
        }
        
        WebAutoClose              WebHtml::i()
        {
            *this << "<i>"sv;
            return WebAutoClose(*this, "</i>"sv);
        }
        
        WebAutoClose              WebHtml::italic()
        {
            *this << "<i>"sv;
            return WebAutoClose(*this, "</i>"sv);
        }
        
        WebAutoClose        WebHtml::kvrow(std::string_view key, const UrlView& url)
        {
            *this << "<tr><th align=\"left\">";
            bool    a   = !url.empty();
            if(a)
                *this << "<a href=\"" << url << "\">";
            html_escape_write(*this, key);
            if(a)
                *this << "</a>";
            *this << "</th><td>";
            return WebAutoClose(*this, "</td></tr>\n"sv);
        }
        
        WebAutoClose              WebHtml::li()
        {
            *this << "<li>"sv;
            return WebAutoClose(*this, "</li>\n"sv);
        }
        
        WebAutoClose        WebHtml::link(const UrlView& url)
        {
            *this << "<a href=\"" << url << "\">";
            return WebAutoClose(*this, "</a>"sv);
        }
        
        WebAutoClose              WebHtml::numbers()
        {
            *this << "<ol>"sv;
            return WebAutoClose(*this, "</ol>"sv);
        }
        
        WebAutoClose              WebHtml::p()
        {
            *this << "<p>"sv;
            return WebAutoClose(*this, "</p>"sv);
        }
        
        WebAutoClose              WebHtml::paragraph()
        {
            *this << "<p>"sv;
            return WebAutoClose(*this, "</p>"sv);
        }
        
        WebAutoClose              WebHtml::pre()
        {
            *this << "<pre>"sv;
            return WebAutoClose(*this, "</pre>"sv);
        }
        
        WebAutoClose        WebHtml::table(std::string_view cls)
        {
            if(cls.empty()){
                *this << "<table>";
            } else {
                *this << "<table class=\"" << cls << "\">";
            }
            return WebAutoClose(*this, "</table>\n"sv);
        }

        WebAutoClose  WebHtml::title()
        {
            Target  oldTarget   = m_target;
            m_target            = TITLE;
            return WebAutoClose(*this, [oldTarget](WebHtml& h){
                h.m_target  = oldTarget;
            });
        }
        
        WebAutoClose              WebHtml::u()
        {
            *this << "<u>"sv;
            return WebAutoClose(*this, "</u>"sv);
        }
        
        WebAutoClose              WebHtml::underline()
        {
            *this << "<u>"sv;
            return WebAutoClose(*this, "</u>"sv);
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
                    yWarning() << "Page '" << name() << "' does NOT start with '*' or '/', bad registeration.";
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
    
    
    WebPage::Writer&  WebPage::Writer::label(std::string_view sv)
    {
        if(m_page && !sv.empty())
            m_page -> m_label = sv;
        return *this;
    }

    WebPage::Writer&  WebPage::Writer::local()
    {
        if(m_page){
        yInfo() << "Page " << m_page->path() << " setting local only";
            m_page -> set_option(LOCAL_ONLY);
            if(!m_page->local_only())
                yWarning() << "Page " << m_page->path() << " failed to set local-only flag";
        }
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
    
    WebPage::Writer& WebPage::Writer::script(std::string_view sv)
    {
        if(m_page)
            m_page -> m_scripts.push_back(sv);
        return *this;
    }

    void    WebPage::Writer::set_group(const WebGroup*g) 
    {
        if(m_page)
            m_page -> m_group = g;
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

    void    WebGroup::add(WebPage*p)
    {
        if(p){
            pages.push_back(p);
            p->m_group = this;
        }
    }
    
    void    reg_webgroup(std::initializer_list<WebPage*> gdef)
    {
        if(!std::empty(gdef)){
            WebGroup*   group   = new WebGroup;
            for(WebPage* p : gdef)
                group -> add(p);
        }
    }

        
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        WebTemplate::WebTemplate(ContentType ct) : m_type(ct)
        {
            parse(szDefaultTemplate);
        }

        WebTemplate::WebTemplate(std::string&& mv, ContentType ct) : m_data(std::move(mv)), m_type(ct)
        {
            parse(m_data);
        }
        
        WebTemplate::WebTemplate(std::string_view k, ContentType ct) : m_data(k), m_type(ct)
        {
            parse(m_data);
        }

        void    WebTemplate::execute(Stream&str, WebContext&ctx) const
        {
            //  TODO .... content type == markdown
        
            for(auto& t : m_bits){
                if(!t.variable){
                    str << t.token;
                    continue;
                }
                
                const WebVariable*  v = web::variable(t.token);
                if(v){
                    v -> handle(str, ctx);
                } else {
                    str << "{{" << t.token << "}}"; // make it clear there's a substitution failure
                }
            }
        }
        

        void    WebTemplate::parse(std::string_view data)
        {
            size_t n   = data.find("{{");
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
                m   = data.find("}}", n);
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
                n   = data.find("{{", m);
            }
            
            if(m < data.size()){
                    // no more, so push the remainder on
                m_bits << Token{ data.substr(m), false };
            }

                // YES, possible to lose between "{{NAME" and end if there's no "}}", but that's ill-formed
            
        }

        void        WebTemplate::writeBitsToInfo() const
        {
            auto write  = [&](log4cpp::CategoryStream&& log)
            {
                log << "WebTemplate Report:\n"
                    << "Variables Found: " << join(m_vars, ", ") << "\n";
                    
                size_t n=1;
                for(auto& b : m_bits){
                    if(b.variable){
                        log << "[" << n << "] Variable: " << b.token << "\n";
                    } else {
                        log << "[" << n << "] Text : " << b.token << "\n";
                    }
                    ++n;
                }
            };
            write(yInfo());
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
        static constexpr const std::string_view     kReplacementChar    = "\xEF\xBF\xBD"sv;
        std::string ret;
        ret.reserve(sv.size());
        iter_http(sv, [&](char c){
            if(!c) [[unlikely]] {
                ret += kReplacementChar;
            } else
                ret += c;
        });
        return ret;
    }

    void            html_escape_write(Stream& s, std::string_view sv)
    {
        static constexpr const std::string_view     kReplacementChar    = "\xEF\xBF\xBD"sv;
        iter_http(sv, [&](char ch){
            if(!ch) [[unlikely]]
                s << kReplacementChar;
            s << ch;
        });
    }
    

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    bool    send_file_info(const std::filesystem::path& file, WebContext& rs, ContentType ct)
    {
        Ref<TypedBytes> tb  = new TypedBytes(file);
        if(!tb -> do_info()){
            rs.status = HttpStatus::NotFound;
            return false;
        }
        
        if(ct == ContentType())
            ct  = tb -> type;
            
        rs.status = HttpStatus::Success;
        rs.tx_header("Date", tb -> modified);
        rs.tx_content_type = ct;
        //rs.header("Content-Length", to_string(tb->size));
        return true;
    }

    bool    send_file(const std::filesystem::path& file, WebContext& rs, ContentType ct)
    {
        Ref<TypedBytes> tb  = new TypedBytes(file);
        if(!tb -> do_info()){
            rs.status = HttpStatus::NotFound;
            return false;
        }
        
        if(!tb -> do_read()){
            rs.status = tb -> status;
            return false;
        }

        if(ct == ContentType())
            ct  = tb -> type;
        
        rs.tx_content_type  = ct;
        rs.tx_content       = std::move(tb->data);
        rs.status = HttpStatus::Success;
        rs.tx_header("Date", tb -> modified);
        return true;
    }
    
    //void    append(NetWriter&, const std::filesystem::path&)
    //{
    //}

    std::string_view    web_title(std::string_view text)
    {
        if(!starts(text, "#!"))
            return std::string_view();
        size_t  n = text.find_first_of('\n');
        if(n == std::string_view::npos)
            return text;
        return std::string_view(text.data()+2, text.data()+n+1);
    }


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    namespace {
       class SimpleWebFileHandler : public WebPage {
        public:
        
            SimpleWebFileHandler(HttpOps m, std::string_view p, const std::source_location& sl, const std::filesystem::path&f) : WebPage(m, p, sl), m_file(f) 
            {
                m_ext       = f.extension().string();
                if(!m_ext.empty() && m_ext[0] == '.')
                    m_ext   = m_ext.substr(1);
            }

            virtual void handle(WebContext&ctx) const override 
            {
                const WebPage*  x   = web::extension(hGet, m_ext);
                if(x){
                    x -> handle(ctx);
                } else {
                    send_file(m_file, ctx);
                }
            }
        
            std::filesystem::path       m_file;
            std::string                 m_ext;
        };
        
        class SimpleWebDirectory : public WebPage {
        public:
            SimpleWebDirectory(HttpOps m, std::string_view p,  const std::source_location& sl, const std::filesystem::path&f) : WebPage(m, p, sl), m_dir(f) 
            {
            }
            
            virtual void handle(WebContext&ctx) const override 
            {
                std::string     p   = path_sanitize(ctx.truncated_path);
                std::filesystem::path   fp  = m_dir / p;
                if(!std::filesystem::exists(fp)){
                    ctx.status = HttpStatus::NotFound;
                    return; 
                } 
                
                auto ext    = file_extension(p);
                if(ext.empty()){
                    send_file(fp, ctx);
                    return ;
                }
            
                const WebPage*  x   = web::extension(hGet, ext);
                if(x){
                    ctx.resolved_file  = fp;
                    x -> handle(ctx);
                } else {
                    send_file(fp, ctx);
                }
            }
        
            std::filesystem::path       m_dir;
        };
     
        class ManyWebDirectory : public WebPage {
        public:
            ManyWebDirectory(HttpOps m, std::string_view p,  const std::source_location& sl, const path_vector_t&f) : WebPage(m, p, sl), m_dirs(f) 
            {
            }
            
            virtual void handle(WebContext&ctx) const override 
            {
                std::string              p = path_sanitize(ctx.truncated_path);
                std::filesystem::path   fp = first(p);
                if(fp.empty()){
                    ctx.status = HttpStatus::NotFound;
                    return; 
                }

                auto ext    = file_extension(p);
                if(ext.empty()){
                    send_file(fp, ctx);
                    return ;
                }
            
                const WebPage*  x   = web::extension(hGet, ext);
                if(x){
                    ctx.resolved_file  = fp;
                    x -> handle(ctx);
                } else {
                    send_file(fp, ctx);
                }
            }
            
            std::filesystem::path       first(std::string_view v) const
            {
                for(auto& d : m_dirs){
                    std::filesystem::path   f   = d / v;
                    if(std::filesystem::exists(f))
                        return f;
                }
                
                return std::filesystem::path();
            }
            
            path_vector_t               m_dirs;
        };
    }

    WebPage::Writer     reg_webpage(const std::string_view& path, const std::filesystem::path&fp, const std::source_location& sl)
    {
        if(std::filesystem::is_directory(fp)){
            return WebPage::Writer( new SimpleWebDirectory(hGet, path, sl, fp));
        } else {
            return WebPage::Writer( new SimpleWebFileHandler(hGet, path, sl, fp));
        }
        
    }
    
    WebPage::Writer     reg_webpage(const std::string_view& path, const path_vector_t&dirs, const std::source_location& sl)
    {
        if(dirs.empty())
            return WebPage::Writer();
        return WebPage::Writer( new ManyWebDirectory(hGet, path, sl, dirs));
    }
    
    //  ------------------------------------------------

    namespace {
        class FunctionWebAdapter : public WebPage {
        public:
            FunctionWebAdapter(HttpOps _methods, std::string_view _path, const std::source_location& sl, std::function<void(WebContext&)> fn) :
                WebPage(_methods, _path, sl), m_fn(fn) {}
                
            virtual void handle(WebContext& ctx) const override
            {
                m_fn(ctx);
            }
            
            std::function<void(WebContext&)>    m_fn;
        };
    }

    WebPage::Writer     reg_webpage(std::string_view path, std::function<void(WebContext&)>fn, const std::source_location& sl)
    {
        return WebPage::Writer( new FunctionWebAdapter(hGet, path, sl, fn));
    }
    
    WebPage::Writer     reg_webpage(HttpOps methods, std::string_view path, std::function<void(WebContext&)>fn, const std::source_location& sl)
    {
        return WebPage::Writer( new FunctionWebAdapter(methods, path, sl, fn));
    }
    
    //  ------------------------------------------------
    namespace {
        class FunctionHtmlAdapter : public WebPage {
        public:
            FunctionHtmlAdapter(HttpOps _methods, std::string_view _path, std::string_view _title, const std::source_location& sl, std::function<void(WebHtml&)> fn) :
                WebPage(_methods, _path, sl), m_fn(fn), m_title(_title) {}
                
            virtual void handle(WebContext& ctx) const override
            {
                WebHtml out(ctx, m_title);
                m_fn(out);
            }
            
            std::function<void(WebHtml&)>       m_fn;
            std::string_view                    m_title;
        };
    }

    WebPage::Writer     reg_webpage(std::string_view path, std::function<void(WebHtml&)> fn, const std::source_location& sl)
    {
        return WebPage::Writer( new FunctionHtmlAdapter(hGet, path, std::string_view(), sl, fn));
    }
    
    WebPage::Writer     reg_webpage(HttpOps methods, std::string_view path, std::function<void(WebHtml&)>fn, const std::source_location& sl)
    {
        return WebPage::Writer( new FunctionHtmlAdapter(methods, path, std::string_view(), sl, fn));
    }
    
    WebPage::Writer     reg_webpage(std::string_view path, std::string_view title, std::function<void(WebHtml&)>fn, const std::source_location& sl)
    {
        return WebPage::Writer( new FunctionHtmlAdapter(hGet, path, title, sl, fn));
    }
    
    WebPage::Writer     reg_webpage(HttpOps methods, std::string_view path, std::string_view title, std::function<void(WebHtml&)>fn, const std::source_location& sl)
    {
        return WebPage::Writer( new FunctionHtmlAdapter(methods, path, title, sl, fn));
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
}
