////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/html/Html.hpp>
#include <yq/mithril/preamble.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&, Leaf);
    WebHtml&    operator<<(WebHtml&, Dev<Leaf>);
    WebHtml&    operator<<(WebHtml&, DevID<Leaf>);
    
    void dev_table(WebHtml&, const LeafVector&);
    void dev_title(WebHtml&, Leaf, std::string_view extra=std::string_view());
}

