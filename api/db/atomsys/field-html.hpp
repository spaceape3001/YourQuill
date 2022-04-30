////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/core/html_common.hpp>

namespace yq {
    struct Field;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Field);
        WebHtml&    operator<<(WebHtml&, Dev<Field>);
        WebHtml&    operator<<(WebHtml&, DevID<Field>);
        void        dev_table(WebHtml&, const std::vector<Field>&);
    }
}


