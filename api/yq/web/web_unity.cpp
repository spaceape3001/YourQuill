////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Web.hpp"

#include <yq/collection/EnumMap.hpp>
#include <yq/collection/Map.hpp>
#include <yq/file/FileUtils.hpp>
#include <yq/http/HttpDataStream.hpp>
#include <yq/http/HttpRequest.hpp>
#include <yq/http/HttpResponse.hpp>
#include <yq/log/Logging.hpp>
#include <yq/text/Utils.hpp>

#include <tbb/spin_rw_mutex.h>

namespace yq {

#define LOCK                                                \
    const auto& _r = repo();                                \
    tbb::spin_rw_mutex::scoped_lock     _lock;              \
    if(_r.open_reg)                                         \
        _lock.acquire(_r.mutex, false);

#define WLOCK                                               \
    auto& _r = repo();                                      \
    tbb::spin_rw_mutex::scoped_lock     _lock(_r.mutex, true);

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    struct Web::Repo {
        using WebPageMap       = EnumMap<HttpOp, Map<std::string_view, const Web*, IgCase>>;
    
        std::vector<const Web*> all;
        WebPageMap                  dirs;
        WebPageMap                  exts;
        WebPageMap                  globs;
        WebPageMap                  pages;
        mutable tbb::spin_rw_mutex  mutex;
        volatile bool               open_reg    = true;
    };
    
    Web::Repo& Web::repo() 
    {
        static Repo s_ret;
        return s_ret;
    }

    const std::vector<const Web*>&  Web::all()
    {
        return repo().all;
    }
    
    const Web*  Web::directory(HttpOp m, std::string_view sv)
    {
        LOCK
        return _r.dirs[m].get(sv, nullptr);
    }
    
    const Web*  Web::extension(HttpOp m, std::string_view sv)
    {
        LOCK
        return _r.exts[m].get(sv, nullptr);
    }
        
    void Web::freeze()
    {
        WLOCK
        _r.open_reg = false;
    }
    
    bool Web::frozen()
    {
        LOCK
        return _r.open_reg;
    }
    
    const Web* Web::glob(HttpOp m, std::string_view sv)
    {
        LOCK
        return _r.globs[m].get(sv, nullptr);
    }

    const Web* Web::page(HttpOp m, std::string_view sv)
    {
        LOCK
        return _r.pages[m].get(sv, nullptr);
    }

    const WebMap&    Web::directory_map()
    {
        return repo().dirs;
    }
    
    const WebMap&    Web::extension_map()
    {
        return repo().exts;
    }
    
    const WebMap&    Web::page_map()
    {
        return repo().pages;
    }
    
    const WebMap&    Web::glob_map()
    {
        return repo().globs;
    }

    //  ----------------------------------------------------------------------------------------------------------------
    
    Web::Web(HttpOps _methods, std::string_view p, const std::source_location& sl) : Meta(p)
    {
        set_option(WEB);
        
        m_path      = p;
        m_methods   = _methods;
        m_source    = sl;
        
        WLOCK
        if(_r.open_reg){
            _r.all.push_back(this);
        }
    }
    
    Web::~Web()
    {
    }

    bool  Web::anonymouse_posting() const
    {
        return static_cast<bool>(flags() & POST_ANON);
    }

    bool  Web::local_only() const
    {
        return static_cast<bool>(flags() & LOCAL_ONLY);
    }
    
    bool  Web::login_required() const
    {
        return static_cast<bool>(flags() & LOGIN_REQ);
    }
    
    bool  Web::no_expansion() const
    {
        return static_cast<bool>(flags() & NO_EXPAND);
    }

    void    Web::seal()
    {
        if(flags() & SEALED)
            return ;
            
        if(!(flags() & DISABLE_REG)){
            std::string_view    p   = m_path;
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
    Web::Writer::Writer(Web*p) : Meta::Writer(p), m_page(p)
    {
    }

    Web::Writer::Writer(Writer&& mv) : Meta::Writer(std::move(mv)), m_page(mv.m_page)
    {
        mv.m_page = nullptr;
    }
    
    Web::Writer& Web::Writer::operator=(Writer&& mv)
    {
        if(this != &mv){
            if(m_page)
                m_page -> seal();
            m_page  = mv.m_page;
            mv.m_page   = nullptr;
        }
        return *this;
    }
    
    Web::Writer::~Writer()
    {
        if(m_page)
            m_page -> seal();
    }

    Web::Writer&  Web::Writer::anon_post()
    {
        if(m_page)
            m_page -> set_option(POST_ANON);
        return *this;
    }

    Web::Writer&  Web::Writer::argument(std::string_view k, std::string_view d) 
    {
        if(m_page)
            m_page -> m_args.push_back( Arg{k, d});
        return *this;
    }
    
    Web::Writer&  Web::Writer::content(ContentTypes ct)
    {
        if(m_page){
            m_page -> m_content_types   |= ct;
        }
        return *this;
    }

    Web::Writer&  Web::Writer::description(std::string_view sv)
    {
        if(m_page)
            Meta::Writer::description(sv);
        return *this;
    }
    
    Web::Writer&  Web::Writer::disable_reg()
    {
        if(m_page)
            m_page -> set_option(DISABLE_REG);
        return *this;
    }
    
    
    Web::Writer&  Web::Writer::local()
    {
        if(m_page)
            m_page -> set_option(LOCAL_ONLY);
        return *this;
    }

    Web::Writer&  Web::Writer::login()
    {
        if(m_page)
            m_page -> set_option(LOGIN_REQ);
        return *this;
    }
    
    
    Web::Writer&  Web::Writer::no_expand()
    {
        if(m_page)
            m_page -> set_option(NO_EXPAND);
        return *this;
    }

    Web::Writer&  Web::Writer::primary(ContentType ct)
    {
        if(m_page){
            m_page -> m_content_type    = ct;
            m_page -> m_content_types   |= ct;
        }
        return *this;
    }
    
    
    Web::Writer& Web::Writer::role(Role r)
    {
        if(m_page){
            m_page -> m_role = r;
        }
        return *this;
    }
    
    Web::Writer& Web::Writer::sub(std::string_view p, const Web*w)
    {
        if(w){
            sub(w->methods(), p, w);
        }
        return *this;
    }
    

    Web::Writer& Web::Writer::sub(HttpOps m, std::string_view p, const Web*w)
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
    
    

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  STANDARD PAGES... (internal classes)

    namespace {
        class StdWebFullHandler : public Web {
        public:
            using FN = std::function<void(const HttpRequest&, HttpResponse&, std::string_view)>;
            FN m_fn;
            StdWebFullHandler(HttpOps m, std::string_view p, const std::source_location& sl, FN fn) : Web(m, p, sl), m_fn(fn) {}
            virtual void handle(const HttpRequest& rq, HttpResponse& rs, std::string_view sv) const override 
            {
                m_fn(rq,rs,sv);
            }
        };
    }

    Web::Writer    reg_web(std::string_view p, std::function<void(const HttpRequest&, HttpResponse&, std::string_view)> fn, const std::source_location& sl)
    {
        return Web::Writer(new StdWebFullHandler( HttpOp::Get, p, sl, fn));
    }
    
    Web::Writer    reg_web(HttpOps m, std::string_view p, std::function<void(const HttpRequest&, HttpResponse&, std::string_view)> fn, const std::source_location& sl)
    {
        return Web::Writer(new StdWebFullHandler( m, p, sl, fn));
    }
    
    //  ------------------------------------------------

    namespace {
        class StdWebSingleHandler : public Web {
        public:
            using FN = std::function<void(const HttpRequest&, HttpResponse&)>;
            FN m_fn;
            StdWebSingleHandler(HttpOps m, std::string_view p, const std::source_location& sl, FN fn) : Web(m, p, sl), m_fn(fn) {}
            virtual void handle(const HttpRequest& rq, HttpResponse& rs, std::string_view) const override 
            {
                m_fn(rq,rs);
            }
        };
    }
    
    Web::Writer    reg_web(std::string_view p, std::function<void(const HttpRequest&, HttpResponse&)> fn, const std::source_location& sl)
    {
        return Web::Writer(new StdWebSingleHandler( HttpOp::Get, p, sl, fn));
    }
    
    Web::Writer    reg_web(HttpOps m, std::string_view p, std::function<void(const HttpRequest&, HttpResponse&)> fn, const std::source_location& sl)
    {
        return Web::Writer(new StdWebSingleHandler( m, p, sl, fn));
    }

    //  ------------------------------------------------


        class WebFileHandler : public Web {
        public:
        
            WebFileHandler(HttpOps m, std::string_view p, const std::source_location& sl, const std::filesystem::path&f) : Web(m, p, sl), m_file(f) {}

            virtual void handle(const HttpRequest& rq, HttpResponse& rs, std::string_view) const override 
            {
                switch(rq.method()){
                case HttpOp::Get:
                    send_file(m_file, rs);
                    break;
                case HttpOp::Head:
                    send_file_info(m_file, rs);
                    break;
                default:
                    throw HttpStatus::MethodNotAllowed;
                }
            }
        
            std::filesystem::path       m_file;
        };


#if 0
    namespace {
    
        class WebDirHandler : public Web {
        public:
            WebDirHandler(HttpOps m, std::string_view p, const std::source_location& sl, const std::filesystem::path& dpath) : Web(m, p, sl), m_dir(dpath){}
            
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
        
        class WebPathVecHandler : public Web {
        public:
            WebPathVecHandler(HttpOps m, std::string_view p, const std::source_location& sl, const path_vector_t& paths) : Web(m, p, sl), m_paths(paths){}
            
            
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

#endif

    Web::Writer     reg_web(std::string_view p, const std::filesystem::path& fs, const std::source_location& sl)
    {
        if(std::filesystem::is_directory(fs)){
            return { nullptr }; // { new WebDirHandler( { HttpOp::Get, HttpOp::Head }, p,sl, fs) };
        } else {
            return { new WebFileHandler( { HttpOp::Get, HttpOp::Head }, p,sl, fs) };
        }
    }

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
    
}
