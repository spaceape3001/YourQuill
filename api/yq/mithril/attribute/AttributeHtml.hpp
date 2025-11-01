////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/html/Html.hpp>
#include <yq/mithril/preamble.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&, Dev<Attribute>);
    WebHtml&    operator<<(WebHtml&, DevID<Attribute>);
    
    void dev_table(WebHtml&, const AttributeVector&);
    void dev_title(WebHtml&, Attribute, std::string_view extra=std::string_view());
}

