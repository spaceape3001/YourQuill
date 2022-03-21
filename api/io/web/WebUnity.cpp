////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "WebPage.hpp"

#include <util/collection/EnumMap.hpp>
#include <util/collection/Map.hpp>
#include <util/log/Logging.hpp>
#include <util/text/Utils.hpp>

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

    struct WebPage::Repo {
        using WebPageMap       = EnumMap<HttpOp, Map<std::string_view, const WebPage*, IgCase>>;
    
        std::vector<const WebPage*> all;
        WebPageMap                  pages;
        mutable tbb::spin_rw_mutex  mutex;
        volatile bool               open_reg    = true;
    };
    
    WebPage::Repo& WebPage::repo() 
    {
        static Repo s_ret;
        return s_ret;
    }

    const std::vector<const WebPage*>&  WebPage::all()
    {
        return repo().all;
    }
        
    void WebPage::freeze()
    {
        WLOCK
        _r.open_reg = false;
    }
    
    bool WebPage::frozen()
    {
        LOCK
        return _r.open_reg;
    }
    
    const WebPage* WebPage::find(HttpOp m, const std::string_view& sv)
    {
        LOCK
        return _r.pages[m].get(sv, nullptr);
    }

    //  ----------------------------------------------------------------------------------------------------------------
    
    WebPage::WebPage(HttpOp m, std::string_view p, const std::source_location& sl) : Meta(p)
    {
        m_path      = p;
        m_method    = m;
        m_source    = sl;
        if(p.empty() || p[0] != '/'){
            yCritical() << "WebPage: Bad path '" << p << "' -- not registering (defined at " << sl.file_name() << ":" << sl.line() << ")"; 
        } else {
            WLOCK
            if(_r.open_reg){
                _r.all.push_back(this);
                _r.pages[m][m_path]     = this;
            }
        }
    }
    
    WebPage::~WebPage()
    {
    }

    bool  WebPage::anonymouse_posting() const
    {
        return static_cast<bool>(m_flags & POST_ANON);
    }

    bool  WebPage::local_only() const
    {
        return static_cast<bool>(m_flags & LOCAL_ONLY);
    }
    
    bool  WebPage::login_required() const
    {
        return static_cast<bool>(m_flags & LOGIN_REQ);
    }
    
    bool  WebPage::no_expansion() const
    {
        return static_cast<bool>(m_flags & NO_EXPAND);
    }

    //  ----------------------------------------------------------------------------------------------------------------
        WebPage::Writer&  WebPage::Writer::login()
        {
            if(m_page)
                m_page -> m_flags |= LOGIN_REQ;
            return *this;
        }
        
        WebPage::Writer&  WebPage::Writer::description(std::string_view sv)
        {
            if(m_page)
                Meta::Writer::description(sv);
            return *this;
        }
        
        WebPage::Writer&  WebPage::Writer::argument(std::string_view k, std::string_view d) 
        {
            if(m_page)
                m_page -> m_args.push_back( Arg{k, d});
            return *this;
        }
        
        WebPage::Writer&  WebPage::Writer::local()
        {
            if(m_page)
                m_page -> m_flags |= LOCAL_ONLY;
            return *this;
        }
        
        WebPage::Writer&  WebPage::Writer::anon_post()
        {
            if(m_page)
                m_page -> m_flags |= POST_ANON;
            return *this;
        }
        
        WebPage::Writer&  WebPage::Writer::no_expand()
        {
            if(m_page)
                m_page -> m_flags |= NO_EXPAND;
            return *this;
        }
        
        WebPage::Writer::Writer(WebPage*p) : Meta::Writer(p), m_page(p)
        {
        }
        

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  STANDARD PAGES... (internal classes)

    class StandardWebPage : public WebPage {
    public:
        using FN = std::function<void(const HttpRequest&, HttpResponse&)>;
        FN m_fn;
        StandardWebPage(HttpOp m, std::string_view p, const std::source_location& sl, FN fn) : WebPage(m, p, sl), m_fn(fn) {}
        virtual void handle(const HttpRequest& rq, HttpResponse& rs) const override 
        {
            m_fn(rq,rs);
        }
    };

    WebPage::Writer    reg_webpage(std::string_view p, std::function<void(const HttpRequest&, HttpResponse&)> fn, const std::source_location& sl)
    {
        return WebPage::Writer(new StandardWebPage( HttpOp::Get, p, sl, fn));
    }
    
    WebPage::Writer    reg_webpage(HttpOp m, std::string_view p, std::function<void(const HttpRequest&, HttpResponse&)> fn, const std::source_location& sl)
    {
        return WebPage::Writer(new StandardWebPage( m, p, sl, fn));
    }
}
