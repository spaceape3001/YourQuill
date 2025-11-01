////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/html/Html.hpp>
#include <yq/mithril/preamble.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&, Organization);
    WebHtml&    operator<<(WebHtml&, Dev<Organization>);
    WebHtml&    operator<<(WebHtml&, DevID<Organization>);
    void        dev_table(WebHtml&, const std::vector<Organization>&);
}
