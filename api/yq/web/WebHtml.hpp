////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/net/Url.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/stream/Stream.hpp>
#include <yq/type/Ref.hpp>

namespace yq {
    struct WebContext;
    class Stream;
    
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
        
        void        title(const std::string_view& _title);
        
        WebContext&         context() { return m_context; }
        const WebContext&   context() const { return m_context; }
        
        void                run_me();
    private:
        WebContext&         m_context;
    };

    template <typename T>
    WebHtml& operator<<(WebHtml& wh, const T& data)
    {
        ((Stream&) wh) << data;
        return wh;
    }

    std::string     html_escape(std::string_view);
    void            html_escape_write(Stream& s, std::string_view);
    
    class WebTag {
    public:
        WebTag(WebHtml&, std::string_view);
        WebTag(WebTag&&);
        WebTag& operator=(WebTag&&);
        ~WebTag();
        
        const std::string_view&     tag() { return m_tag; }
    
        template <typename T>
        WebTag&     operator<<(T data)
        {
            if(m_html)
                *m_html << data;
            return *this;
        }
        
    private:
        WebTag(const WebTag&) = delete;
        WebTag& operator=(const WebTag&) = delete;
    
        void    close();
    
        WebHtml*            m_html;
        std::string_view    m_tag;
    };
    
    class WebAutoClose {
    public:
        WebAutoClose(WebHtml&, std::string_view);
        WebAutoClose(WebAutoClose&&);
        WebAutoClose& operator=(WebAutoClose&&);
        ~WebAutoClose();
    
        template <typename T>
        WebAutoClose&     operator<<(T data)
        {
            if(m_html)
                *m_html << data;
            return *this;
        }

        const std::string_view&     text() { return m_text; }

    private:
        WebAutoClose(const WebAutoClose&) = delete;
        WebAutoClose& operator=(const WebAutoClose&) = delete;
    
        void    close();
    
        WebHtml*            m_html;
        std::string_view    m_text;
    };


    namespace html {
        //! Bold whatever comes next
        WebTag   bold(WebHtml& wh);
        
        //! Unordeered list
        WebTag  bullets(WebHtml&);

        //! Header 1 for whatever comes next
        WebTag   h1(WebHtml& wh);

        //! Header 2 for whatever comes next
        WebTag   h2(WebHtml& wh);

        //! Header 3 for whatever comes next
        WebTag   h3(WebHtml& wh);

        //! Header 4 for whatever comes next
        WebTag   h4(WebHtml& wh);

        //! Header 5 for whatever comes next
        WebTag   h5(WebHtml& wh);

        //! Header 6 for whatever comes next
        WebTag   h6(WebHtml& wh);
        
        //! Italic whatever comes next
        WebTag   italic(WebHtml& wh);
        
        
        //! Used for two-column key/value tables
        WebAutoClose  kvrow(WebHtml&, std::string_view key, const UrlView& url= UrlView());
        WebTag  li(WebHtml&);
        WebAutoClose  link(WebHtml&, const UrlView& uri);
        
        //! Numbered (ordered) list
        WebTag  numbers(WebHtml&);
        
        
        //! Paragraph for whatever comes next
        WebTag   paragraph(WebHtml& wh);
        
        //! Preformatted text for whatever comes next
        WebTag   pre(WebHtml& wh);
        
        //! Table for whatever comes next
        WebAutoClose    table(WebHtml& wh, std::string_view cls=std::string_view());
        
        //! Underline for whatever comes next
        WebTag   underline(WebHtml& wh);
    }
    
}
