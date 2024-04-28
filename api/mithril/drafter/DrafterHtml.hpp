////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/html/Html.hpp>

namespace yq::mithril::html {
    //WebHtml&    operator<<(WebHtml&, Drafter);
    WebHtml&    operator<<(WebHtml&, Dev<Drafter>);
    WebHtml&    operator<<(WebHtml&, DevID<Drafter>);

    void dev_table(WebHtml&, const std::vector<Drafter>&);
    void dev_title(WebHtml&, Drafter, std::string_view extra=std::string_view());
}


