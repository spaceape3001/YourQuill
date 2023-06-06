////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>

namespace yq::mithril {
    struct Place;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Place);
        WebHtml&    operator<<(WebHtml&, Dev<Place>);
        WebHtml&    operator<<(WebHtml&, DevID<Place>);
        void        dev_table(WebHtml&, const std::vector<Place>&);
        
    }
}
