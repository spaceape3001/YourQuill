////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>
#include <mithril/preamble.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&, Character);
    WebHtml&    operator<<(WebHtml&, Dev<Character>);
    WebHtml&    operator<<(WebHtml&, DevID<Character>);
    void        dev_table(WebHtml&, const std::vector<Character>&);
}
