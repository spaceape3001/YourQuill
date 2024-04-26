////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>
#include <vector>

namespace yq::mithril {
    struct Style;
    namespace html {
        //WebHtml&    operator<<(WebHtml&, Style);
        WebHtml&    operator<<(WebHtml&, Dev<Style>);
        WebHtml&    operator<<(WebHtml&, DevID<Style>);
        void        dev_table(WebHtml&, const std::vector<Style>&);
    }
}


