////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/html/Html.hpp>

namespace yq::mithril {
    struct Thing;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Thing);
        WebHtml&    operator<<(WebHtml&, Dev<Thing>);
        WebHtml&    operator<<(WebHtml&, DevID<Thing>);
        void        dev_table(WebHtml&, const std::vector<Thing>&);
        
    }
}
