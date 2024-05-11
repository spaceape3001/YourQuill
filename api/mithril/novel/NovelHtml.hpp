////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>
#include <mithril/preamble.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&, Novel);
    WebHtml&    operator<<(WebHtml&, Dev<Novel>);
    WebHtml&    operator<<(WebHtml&, DevID<Novel>);
    
    void dev_table(WebHtml&, const NovelVector&);
    void dev_title(WebHtml&, Novel, std::string_view extra=std::string_view());
}

