////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/preamble.hpp>
#include <yq/mithril/html/Html.hpp>

namespace yq::mithril::html {
    //WebHtml&    operator<<(WebHtml&, Doodle);
    WebHtml&    operator<<(WebHtml&, Dev<Doodle>);
    WebHtml&    operator<<(WebHtml&, DevID<Doodle>);

    void dev_table(WebHtml&, const std::vector<Doodle>&);
    void dev_title(WebHtml&, Doodle, std::string_view extra=std::string_view());
}


