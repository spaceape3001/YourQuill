////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/html_common.hpp>

namespace yq {
    struct User;
    namespace html {
        WebHtml&    operator<<(WebHtml&, User);
        WebHtml&    operator<<(WebHtml&, Dev<User>);
        WebHtml&    operator<<(WebHtml&, DevID<User>);
        void        dev_table(WebHtml&, const std::vector<User>&);
    }
}


