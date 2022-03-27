#pragma once

#include <yq/stream/Ops.hpp>
#include <yq/stream/Stream.hpp>
#include <yq/type/Ref.hpp>

namespace yq {
    struct WebContext;
    class Stream;
    

    class WebHtml : public Stream {
    public:
    
        typedef void    (*FNGetter)(Stream&, WebContext&);
    
        static void     set_template(const std::filesystem::path&);
        static void     reg_getter(const std::string_view&, FNGetter);
        
        WebHtml(WebContext&, const std::string_view& _title);
        ~WebHtml();
        
        bool write(const char* buf, size_t cb) override;
        bool is_open() const override { return true; }

    private:
    
        void                run_me();
        WebContext&         m_context;
        std::string         m_title;
        std::string         m_body;
    };

    template <typename T>
    WebHtml& operator<<(WebHtml& wh, T data)
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
    
    namespace html {
        inline WebTag   p(WebHtml& wh)
        {
            return WebTag(wh, "p"sv);
        }
    }
    
}
