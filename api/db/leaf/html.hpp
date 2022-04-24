////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/html_common.hpp>

namespace yq {
    struct Leaf;
    namespace html {
        //WebHtml&    operator<<(WebHtml&, Leaf);
        //WebHtml&    operator<<(WebHtml&, Dev<Leaf>);
        WebHtml&    operator<<(WebHtml&, DevID<Leaf>);
        //void        dev_table(WebHtml&, const std::vector<Leaf>&);
    }
}


