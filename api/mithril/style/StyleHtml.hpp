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

        void        admin_table(WebHtml&, const StyleVector&);
        void        dev_table(WebHtml&, const std::vector<Style>&);

        void        new_style_control(WebHtml&, std::string_view npath);
    }
}


