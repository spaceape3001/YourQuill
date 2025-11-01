////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>
#include <mithril/preamble.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&, Book);
    WebHtml&    operator<<(WebHtml&, Dev<Book>);
    WebHtml&    operator<<(WebHtml&, DevID<Book>);
    void        dev_table(WebHtml&, const std::vector<Book>&);
}
