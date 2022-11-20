////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <http/Html.hpp>
#include <kernel/agw/Class.hpp>

namespace yq {
    namespace agw {
        struct Field;
    }
    namespace html {
        WebHtml&    operator<<(WebHtml&, agw::Field);
        WebHtml&    operator<<(WebHtml&, Dev<agw::Field>);
        WebHtml&    operator<<(WebHtml&, DevID<agw::Field>);
        void        admin_table(WebHtml&, const std::vector<agw::Field>&);
        void        new_field_control(WebHtml&, std::string_view npath, agw::Class c=agw::Class{});
        void        dev_table(WebHtml&, const std::vector<agw::Field>&);
    }
}


