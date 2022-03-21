////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Web.hpp"

#include <yq/collection/EnumMap.hpp>
#include <yq/collection/Map.hpp>
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
    
    const Web* Web::find(HttpOp m, const std::string_view& sv)
    {
        LOCK
        return _r.pages[m].get(sv, nullptr);
    }

    //  ----------------------------------------------------------------------------------------------------------------
    
    Web::Web(Flag<HttpOp> _methods, std::string_view p, const std::source_location& sl) : Meta(p)
    {
        m_path      = p;
        if(ends(p, "/*"))
            m_flags |= DIRECTORY;
        if(ends(p, "/**"))
            m_flags |= DIRECTORY | RECURSIVE ;
        
        m_method    = _methods;
        m_source    = sl;
        if(p.empty() || p[0] != '/'){
            yCritical() << "Web: Bad path '" << p << "' -- not registering (defined at " << sl.file_name() << ":" << sl.line() << ")"; 
        } else {
            WLOCK
            if(_r.open_reg){
                _r.all.push_back(this);
                for(HttpOp m : HttpOp::all_values()){
                    if(_methods.is_set(m))
                        _r.pages[m][m_path]     = this;
                }
            }
        }
    }
    
    Web::~Web()
    {
    }

    bool  Web::anonymouse_posting() const
    {
        return static_cast<bool>(m_flags & POST_ANON);
    }

    bool  Web::local_only() const
    {
        return static_cast<bool>(m_flags & LOCAL_ONLY);
    }
    
    bool  Web::login_required() const
    {
        return static_cast<bool>(m_flags & LOGIN_REQ);
    }
    
    bool  Web::no_expansion() const
    {
        return static_cast<bool>(m_flags & NO_EXPAND);
    }

    bool    Web::is_directory() const
    {
        return static_cast<bool>(m_flags & DIRECTORY);
    }
    
    bool    Web::is_recursive() const
    {
        return static_cast<bool>(m_flags & RECURSIVE);
    }

    //  ----------------------------------------------------------------------------------------------------------------
    Web::Writer&  Web::Writer::login()
    {
        if(m_page)
            m_page -> m_flags |= LOGIN_REQ;
        return *this;
    }
    
    Web::Writer&  Web::Writer::description(std::string_view sv)
    {
        if(m_page)
            Meta::Writer::description(sv);
        return *this;
    }
    
    Web::Writer&  Web::Writer::argument(std::string_view k, std::string_view d) 
    {
        if(m_page)
            m_page -> m_args.push_back( Arg{k, d});
        return *this;
    }
    
    Web::Writer&  Web::Writer::local()
    {
        if(m_page)
            m_page -> m_flags |= LOCAL_ONLY;
        return *this;
    }
    
    Web::Writer&  Web::Writer::anon_post()
    {
        if(m_page)
            m_page -> m_flags |= POST_ANON;
        return *this;
    }
    
    Web::Writer&  Web::Writer::no_expand()
    {
        if(m_page)
            m_page -> m_flags |= NO_EXPAND;
        return *this;
    }
    
    Web::Writer::Writer(Web*p) : Meta::Writer(p), m_page(p)
    {
    }
    

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  STANDARD PAGES... (internal classes)

    class StdWebFull : public Web {
    public:
        using FN = std::function<void(const HttpRequest&, HttpResponse&, const std::string_view&)>;
        FN m_fn;
        StdWebFull(Flag<HttpOp> m, std::string_view p, const std::source_location& sl, FN fn) : Web(m, p, sl), m_fn(fn) {}
        virtual void handle(const HttpRequest& rq, HttpResponse& rs, const std::string_view&sv) const override 
        {
            m_fn(rq,rs,sv);
        }
    };

    Web::Writer    reg_web(std::string_view p, std::function<void(const HttpRequest&, HttpResponse&, const std::string_view&)> fn, const std::source_location& sl)
    {
        return Web::Writer(new StdWebFull( HttpOp::Get, p, sl, fn));
    }
    
    Web::Writer    reg_web(Flag<HttpOp> m, std::string_view p, std::function<void(const HttpRequest&, HttpResponse&, const std::string_view&)> fn, const std::source_location& sl)
    {
        return Web::Writer(new StdWebFull( m, p, sl, fn));
    }
    
    //  ------------------------------------------------

    class StdWebSingle : public Web {
    public:
        using FN = std::function<void(const HttpRequest&, HttpResponse&)>;
        FN m_fn;
        StdWebSingle(Flag<HttpOp> m, std::string_view p, const std::source_location& sl, FN fn) : Web(m, p, sl), m_fn(fn) {}
        virtual void handle(const HttpRequest& rq, HttpResponse& rs, const std::string_view&) const override 
        {
            m_fn(rq,rs);
        }
    };

    Web::Writer    reg_web(std::string_view p, std::function<void(const HttpRequest&, HttpResponse&)> fn, const std::source_location& sl)
    {
        return Web::Writer(new StdWebSingle( HttpOp::Get, p, sl, fn));
    }
    
    Web::Writer    reg_web(Flag<HttpOp> m, std::string_view p, std::function<void(const HttpRequest&, HttpResponse&)> fn, const std::source_location& sl)
    {
        return Web::Writer(new StdWebSingle( m, p, sl, fn));
    }



    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    bool    send_file(const std::filesystem::path& file, HttpResponse& rs, ContentType ct)
    {
        if(ct == ContentType()){
            ct = mimeTypeForExt(file.extension().string().substr(1));
        }
    
        struct stat buf;
        if(::stat(file.c_str(), &buf) != 0)
            return false;
            
            //  we don't do zero sized files
        size_t  fsize   = buf.st_size;
        if(!fsize)
            return false;
        
        
        char        timestamp[256];
        {
            struct tm   ftime;
            gmtime_r(&buf.st_mtim.tv_sec, &ftime);
            strftime(timestamp, sizeof(timestamp), "%a, %d %b %Y %H:%M:%s GMT", &ftime);
        }
        rs.header("Date", timestamp);

        int fd   = open(file.c_str(), O_RDONLY);
        if(fd == -1)
            return false;

        std::vector<HttpDataPtr>&   data    = rs.content(ct);
        
        size_t          amt_read    = 0;
        HttpDataPtr     cur     = HttpData::make();
        ssize_t         n = 0;
        while((n = read(fd, cur->freespace(), cur->available())) > 0){
            cur -> advance(n);
            amt_read += n;
            if(cur->full()){
                data.push_back(cur);
                if(amt_read >= fsize){
                    cur = nullptr;
                    break;
                }
                cur = HttpData::make();
            }
        } 
        close(fd);
        if(cur)
            data.push_back(cur);
        if(amt_read < fsize)
            rs.status(HttpStatus::InternalError);
        else
            rs.status(HttpStatus::Success);
        return true;
    }
    
}
