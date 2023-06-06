////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>
#include <vector>

namespace yq::mithril {
    struct Attribute;
    
    namespace html {
        WebHtml&    operator<<(WebHtml&, Dev<Attribute>);
        WebHtml&    operator<<(WebHtml&, DevID<Attribute>);
        void    dev_table(WebHtml&, const std::vector<Attribute>&);
    }
}

