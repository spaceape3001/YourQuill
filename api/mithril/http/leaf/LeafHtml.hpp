////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/http/Html.hpp>
#include <vector>

namespace yq::mithril {
    struct Leaf;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Leaf);
        WebHtml&    operator<<(WebHtml&, Dev<Leaf>);
        WebHtml&    operator<<(WebHtml&, DevID<Leaf>);
        void        dev_table(WebHtml&, const std::vector<Leaf>&);
    }
}


