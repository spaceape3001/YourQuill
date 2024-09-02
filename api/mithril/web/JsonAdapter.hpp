////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "WebPage.hpp"
#include "WebContext.hpp"
#include <yq/typedef/json.hpp>
#include <nlohmann/json.hpp>

namespace yq::mithril {

    template <json (*FN)(WebContext&)>
    class JSONWebAdapter : public WebPage {
    public:
        JSONWebAdapter(HttpOps _methods, std::string_view _path, const std::source_location& _sl) : WebPage(_methods, _path, _sl)
        {
        }
        
        virtual void handle(WebContext& ctx) const override
        {
            json j  = FN(ctx);
            
            ctx.tx_content  = std::make_shared<ByteArray>();
            ::nlohmann::detail::serializer<json> s(::nlohmann::detail::output_adapter<char>(ctx.tx_content -> m_data), ' ');
            s.dump(j, false, false, 0);
            ctx.tx_content_type = ContentType::json;
        }
    };

    template <json (*FN)(WebContext&)>
    WebPage::Writer     reg_webpage(std::string_view path, const std::source_location& sl = std::source_location::current())
    {
        return WebPage::Writer( new JSONWebAdapter<FN>(hGet, path, sl) );
    }
    
    template <json (*FN)(WebContext&)>
    WebPage::Writer     reg_webpage(HttpOps methods, std::string_view path, const std::source_location& sl = std::source_location::current())
    {
        return WebPage::Writer( new JSONWebAdapter<FN>(methods, path, sl) );
    }
}
