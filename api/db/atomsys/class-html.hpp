////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/core/html_common.hpp>

namespace yq {
    struct Class;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Class);
        WebHtml&    operator<<(WebHtml&, Dev<Class>);
        WebHtml&    operator<<(WebHtml&, DevID<Class>);
        void        dev_table(WebHtml&, const std::vector<Class>&);
    }
}


