////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/core/html_common.hpp>
#include <vector>

namespace yq {
    struct Tag;
    namespace html {
        //WebHtml&    operator<<(WebHtml&, Tag);
        WebHtml&    operator<<(WebHtml&, Dev<Tag>);
        WebHtml&    operator<<(WebHtml&, DevID<Tag>);
        void        dev_table(WebHtml&, const std::vector<Tag>&);
    }
}


