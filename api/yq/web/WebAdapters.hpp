////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
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

    template <void (*FN)(Stream&, WebContext&)>
    class SimpleWebVariable : public WebVariable {
    public:
        SimpleWebVariable(std::string_view _path, const std::source_location& _sl) : WebVariable(_path, _sl)
        {
        }
        
        
        virtual void handle(Stream&str, WebContext&ctx) const override
        {
            FN(str, ctx);
        }
    };
    
    template <void (*FN)(Stream&, WebContext&)>
    WebVariable::Writer     reg_webvar(std::string_view name, const std::source_location& sl = std::source_location::current())
    {   
        return WebVariable::Writer(new SimpleWebVariable<FN>(name, sl));
    }
    
    //  it'll select file vs dir based on filesystem path
    WebPage::Writer     reg_web(const std::string_view& path, const std::filesystem::path&, const std::source_location& sl = std::source_location::current());
    WebPage::Writer     reg_web(const std::string_view& path, const path_vector_t&, const std::source_location& sl = std::source_location::current());
}

