////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/db/Html.hpp>
#include <kernel/atom/Class.hpp>

namespace yq {
    struct Field;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Field);
        WebHtml&    operator<<(WebHtml&, Dev<Field>);
        WebHtml&    operator<<(WebHtml&, DevID<Field>);
        void        admin_table(WebHtml&, const std::vector<Field>&);
        void        new_field_control(WebHtml&, std::string_view npath, Class c=Class{});
        void        dev_table(WebHtml&, const std::vector<Field>&);
    }
}


