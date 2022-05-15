////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/db/Html.hpp>

namespace yq {
    struct User;
    namespace html {
        WebHtml&    operator<<(WebHtml&, User);
        WebHtml&    operator<<(WebHtml&, Dev<User>);
        WebHtml&    operator<<(WebHtml&, DevID<User>);
        void        admin_table(WebHtml&, const std::vector<User>&);
        void        dev_table(WebHtml&, const std::vector<User>&);
        void        new_user_control(WebHtml&, std::string_view npath);
    }
}


