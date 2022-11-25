////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "WebHtml.hpp"
#include "WebPage.hpp"
#include "WebVariable.hpp"

namespace yq {

    template <void (*FN)(WebContext&)>
    class SimpleWebAdapter : public WebPage {
    public:
        SimpleWebAdapter(HttpOps _methods, std::string_view _path, const std::source_location& _sl) : WebPage(_methods, _path, _sl)
        {
        }
        
        virtual void handle(WebContext& ctx) const override
        {
            FN(ctx);
        }
    };

    template <void (*FN)(WebContext&)>
    WebPage::Writer     reg_webpage(std::string_view path, const std::source_location& sl = std::source_location::current())
    {
        return WebPage::Writer( new SimpleWebAdapter<FN>(hGet, path, sl) );
    }
    
    template <void (*FN)(WebContext&)>
    WebPage::Writer     reg_webpage(HttpOps methods, std::string_view path, const std::source_location& sl = std::source_location::current())
    {
        return WebPage::Writer( new SimpleWebAdapter<FN>(methods, path, sl) );
    }
    
    WebPage::Writer     reg_webpage(std::string_view path, std::function<void(WebContext&)>, const std::source_location& sl = std::source_location::current());
    WebPage::Writer     reg_webpage(HttpOps methods, std::string_view path, std::function<void(WebContext&)>, const std::source_location& sl = std::source_location::current());
    

    template <void (*FN)(WebHtml&)>
    class SimpleWebVariable : public WebVariable {
    public:
        SimpleWebVariable(std::string_view _path, const std::source_location& _sl) : WebVariable(_path, _sl)
        {
        }
        
        
        virtual void handle(WebHtml&ctx) const override
        {
            FN(ctx);
        }
    };
    
    template <void (*FN)(WebHtml&)>
    WebVariable::Writer     reg_webvar(std::string_view name, const std::source_location& sl = std::source_location::current())
    {   
        return WebVariable::Writer(new SimpleWebVariable<FN>(name, sl));
    }
    
    //  it'll select file vs dir based on filesystem path
    WebPage::Writer     reg_webpage(const std::string_view& path, const std::filesystem::path&, const std::source_location& sl = std::source_location::current());
    WebPage::Writer     reg_webpage(const std::string_view& path, const path_vector_t&, const std::source_location& sl = std::source_location::current());

    template <void (*FN)(WebHtml&)>
    class SimpleHtmlAdapter : public WebPage {
    public:
        SimpleHtmlAdapter(HttpOps _methods, std::string_view _path, std::string_view _title, const std::source_location& _sl) : WebPage(_methods, _path, _sl), m_title(_title)
        {
        }
        
        virtual void handle(WebContext& ctx) const override
        {
            WebHtml out(ctx, m_title);
            FN(out);
        }
        
        std::string_view    m_title;
    };


    template <void (*FN)(WebHtml&)>
    WebPage::Writer     reg_webpage(std::string_view path, const std::source_location& sl = std::source_location::current())
    {
        return WebPage::Writer( new SimpleHtmlAdapter<FN>(hGet, path, std::string_view(), sl) );
    }
    
    template <void (*FN)(WebHtml&)>
    WebPage::Writer     reg_webpage(HttpOps methods, std::string_view path, const std::source_location& sl = std::source_location::current())
    {
        return WebPage::Writer( new SimpleHtmlAdapter<FN>(methods, path, std::string_view(), sl) );
    }

    template <void (*FN)(WebHtml&)>
    WebPage::Writer     reg_webpage(std::string_view path, std::string_view title, const std::source_location& sl = std::source_location::current())
    {
        return WebPage::Writer( new SimpleHtmlAdapter<FN>(hGet, path, title, sl) );
    }
    
    template <void (*FN)(WebHtml&)>
    WebPage::Writer     reg_webpage(HttpOps methods, std::string_view path, std::string_view title, const std::source_location& sl = std::source_location::current())
    {
        return WebPage::Writer( new SimpleHtmlAdapter<FN>(methods, path, title, sl) );
    }

    WebPage::Writer     reg_webpage(std::string_view path, std::function<void(WebHtml&)>, const std::source_location& sl = std::source_location::current());
    WebPage::Writer     reg_webpage(HttpOps methods, std::string_view path, std::function<void(WebHtml&)>, const std::source_location& sl = std::source_location::current());
    WebPage::Writer     reg_webpage(std::string_view path, std::string_view title, std::function<void(WebHtml&)>, const std::source_location& sl = std::source_location::current());
    WebPage::Writer     reg_webpage(HttpOps methods, std::string_view path, std::string_view title, std::function<void(WebHtml&)>, const std::source_location& sl = std::source_location::current());

    void                reg_webgroup(std::initializer_list<WebPage*>);


    //! Decodes (%) of the web query & parses it into a multimap
    StringMultiMap      decode_web_query(std::string_view);
}

