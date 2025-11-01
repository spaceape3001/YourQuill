////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/html/Html.hpp>
#include <yq/mithril/preamble.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&, Character);
    WebHtml&    operator<<(WebHtml&, Dev<Character>);
    WebHtml&    operator<<(WebHtml&, DevID<Character>);
    void        dev_table(WebHtml&, const std::vector<Character>&);
}
