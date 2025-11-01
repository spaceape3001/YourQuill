////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/html/Html.hpp>
#include <yq/mithril/preamble.hpp>

namespace yq::mithril::html {
    //WebHtml&    operator<<(WebHtml&, Field);
    //WebHtml&    operator<<(WebHtml&, Dev<Field>);
    //WebHtml&    operator<<(WebHtml&, DevID<Field>);
    //void        admin_table(WebHtml&, const std::vector<Field>&);
    //void        new_field_control(WebHtml&, std::string_view npath, Class c=Class{});
    //void        dev_table(WebHtml&, const std::vector<Field>&);
    void dev_title(WebHtml&, Value, std::string_view extra=std::string_view());
}

