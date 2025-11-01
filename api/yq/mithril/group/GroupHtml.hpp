////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>

namespace yq::mithril {
    struct Group;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Group);
        WebHtml&    operator<<(WebHtml&, Dev<Group>);
        WebHtml&    operator<<(WebHtml&, DevID<Group>);
        void        dev_table(WebHtml&, const std::vector<Group>&);
        
    }
}
