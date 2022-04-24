////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/html_common.hpp>

namespace yq {
    struct Tag;
    namespace html {
        //WebHtml&    operator<<(WebHtml&, Tag);
        WebHtml&    operator<<(WebHtml&, Dev<Tag>);
        WebHtml&    operator<<(WebHtml&, DevID<Tag>);
        void        dev_table(WebHtml&, const std::vector<Tag>&);
    }
}


