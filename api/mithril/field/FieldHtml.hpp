////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>
#include <mithril/class/Class.hpp>

namespace yq::mithril {
    struct Field;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Field);
        WebHtml&    operator<<(WebHtml&, Dev<Field>);
        WebHtml&    operator<<(WebHtml&, const std::set<Field>&);
        WebHtml&    operator<<(WebHtml&, const std::vector<Field>&);
        WebHtml&    operator<<(WebHtml&, const Dev<std::set<Field>>&);
        WebHtml&    operator<<(WebHtml&, const Dev<std::vector<Field>>&);
        WebHtml&    operator<<(WebHtml&, DevID<Field>);
        void        admin_table(WebHtml&, const std::vector<Field>&);
        void        new_field_control(WebHtml&, std::string_view npath, Class c=Class{});
        void        dev_table(WebHtml&, const std::vector<Field>&);
    }
}


