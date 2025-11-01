////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>
#include <mithril/preamble.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&, Entity);
    WebHtml&    operator<<(WebHtml&, Dev<Entity>);
    WebHtml&    operator<<(WebHtml&, DevID<Entity>);
    void        dev_table(WebHtml&, const std::vector<Entity>&);
}
