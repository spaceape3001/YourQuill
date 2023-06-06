////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>

namespace yq::mithril {
    struct Event;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Event);
        WebHtml&    operator<<(WebHtml&, Dev<Event>);
        WebHtml&    operator<<(WebHtml&, DevID<Event>);
        void        dev_table(WebHtml&, const std::vector<Event>&);
    }
}
