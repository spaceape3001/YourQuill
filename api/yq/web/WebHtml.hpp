////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/net/Url.hpp>
#include <yq/stream/Ops.hpp>
#include <basic/Stream.hpp>
#include <basic/ByteArray.hpp>
#include <basic/Ref.hpp>
#include <functional>

namespace yq {
    struct WebContext;
    class Stream;
    class ByteArray;
    struct Root;
    
    class WebAutoClose;
    
    
    /*! \brief Web Title
    
        Detects a "web-title" syntax in the document, denoted by the start with a "#!" syntax.  
        Returns empty if not found.
        
        \note This is NOT trimmed, it will typically have whitespace in the trailing newline.
    */
    std::string_view    web_title(std::string_view);

    class WebHtml : public Stream {
    public:

        typedef void    (*FNGetter)(Stream&, WebContext&);
    
        static void     set_template(const std::filesystem::path&);
        static void     reg_getter(const std::string_view&, FNGetter);
        
        WebHtml(WebContext&, const std::string_view& _title);
        ~WebHtml();
        
        bool write(const char* buf, size_t cb) override;
        bool is_open() const override { return true; }
        
        void      title(const std::string_view& _title);
        
        WebContext&         context() { return m_context; }
        const WebContext&   context() const { return m_context; }
        
        template <typename T>
        WebAutoClose        edit(T, const Root* r=nullptr, bool forceInspect=false);
        
        void      run_me();
        
        //  HTML helpers (will replace the below...)
        WebAutoClose  b();
        WebAutoClose  bold();
        WebAutoClose  bullets();
        WebAutoClose  h1();
        WebAutoClose  h2();
        WebAutoClose  h3();
        WebAutoClose  h4();
        WebAutoClose  h5();
        WebAutoClose  h6();
 
        void      h1(std::string_view);
        void      h2(std::string_view);
        void      h3(std::string_view);
        void      h4(std::string_view);
        void      h5(std::string_view);
        void      h6(std::string_view);
 
        WebAutoClose  i();
        WebAutoClose  italic();
        WebAutoClose  kvrow(std::string_view key, const UrlView& url= UrlView());
        WebAutoClose  li();
        WebAutoClose  link(const UrlView& uri);
        WebAutoClose  numbers();
        WebAutoClose  p();
        WebAutoClose  paragraph();
        WebAutoClose  pre();
        WebAutoClose  table(std::string_view cls=std::string_view());
        WebAutoClose  title();
        WebAutoClose  u();
        WebAutoClose  underline();
        
        //  called to script (will be pushed to the end of the document)
        WebAutoClose  script();
        
        operator const WebContext& () const noexcept { return m_context; }
        operator WebContext& () noexcept { return m_context; }
        
    private:
        WebContext&         m_context;
        ByteArray*          m_dest = nullptr;
        enum Target {
            BODY,
            TITLE,
            SCRIPT,
            DEST
        };
        Target              m_target     = BODY;
    };

    template <typename T>
    WebHtml& operator<<(WebHtml& wh, const T& data)
    {
        ((Stream&) wh) << data;
        return wh;
    }

    std::string     html_escape(std::string_view);
    void            html_escape_write(Stream& s, std::string_view);
    
    
    class WebAutoClose {
    public:
    
        using CloseHandler  = std::function<void(WebHtml&)>;

        WebAutoClose(WebHtml&, std::string_view );

        WebAutoClose(WebHtml&, CloseHandler);
        WebAutoClose(WebAutoClose&&);
        WebAutoClose&   operator=(WebAutoClose&&);
        ~WebAutoClose();

        template <typename T>
        WebAutoClose&     operator<<(const T& data)
        {
            if(m_html)
                *m_html << data;
            return *this;
        }
    
    protected:
        friend class WebHtml;
        WebHtml*        m_html  = nullptr;
        CloseHandler    m_close;

    private:
        WebAutoClose(const WebAutoClose&) = delete;
        WebAutoClose& operator=(const WebAutoClose&) = delete;
        void                    close();
    };
    
}
