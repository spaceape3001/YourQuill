////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/html/Html.hpp>
#include <yq/mithril/preamble.hpp>
#include <yq/mithril/enum/SizeDesc.hpp>

namespace yq::mithril {
    struct Thumbnail;
}

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&, Image);
    WebHtml&    operator<<(WebHtml&, Dev<Image>);
    WebHtml&    operator<<(WebHtml&, DevID<Image>);
    WebHtml&    operator<<(WebHtml&, const Thumbnail&);
    WebHtml&    operator<<(WebHtml&, SizeDesc);
    
    void dev_table(WebHtml&, const ImageVector&);
    void dev_title(WebHtml&, Image, std::string_view extra=std::string_view());
}

