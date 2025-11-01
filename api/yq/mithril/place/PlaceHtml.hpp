////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/html/Html.hpp>

namespace yq::mithril {
    struct Place;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Place);
        WebHtml&    operator<<(WebHtml&, Dev<Place>);
        WebHtml&    operator<<(WebHtml&, DevID<Place>);
        void        dev_table(WebHtml&, const PlaceVector&);
        
    }
}
