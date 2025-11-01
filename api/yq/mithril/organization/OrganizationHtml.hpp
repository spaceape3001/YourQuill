////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>
#include <mithril/preamble.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&, Organization);
    WebHtml&    operator<<(WebHtml&, Dev<Organization>);
    WebHtml&    operator<<(WebHtml&, DevID<Organization>);
    void        dev_table(WebHtml&, const std::vector<Organization>&);
}
