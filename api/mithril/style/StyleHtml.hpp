////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/html/Html.hpp>

namespace yq::mithril::html {
    //WebHtml&    operator<<(WebHtml&, Style);
    WebHtml&    operator<<(WebHtml&, Dev<Style>);
    WebHtml&    operator<<(WebHtml&, DevID<Style>);

    void admin_table(WebHtml&, const StyleVector&);
    void dev_table(WebHtml&, const std::vector<Style>&);
    void dev_title(WebHtml&, Style, std::string_view extra=std::string_view());
    void new_style_control(WebHtml&, std::string_view npath);
}


