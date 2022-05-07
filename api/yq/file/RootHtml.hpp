////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/db/Html.hpp>

namespace yq {
    struct Root;
    namespace html {
        WebHtml&    operator<<(WebHtml&, const Root*);
        WebHtml&    operator<<(WebHtml&, Dev<const Root*>);
        WebHtml&    operator<<(WebHtml&, DevID<const Root*>);
        void        dev_table(WebHtml&, const std::vector<const Root*>&);
    }
}


