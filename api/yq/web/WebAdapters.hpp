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
        SimpleWebVariable(std::string_view _path) : WebVariable(_path)
        {
        }
        
    
    };
}

