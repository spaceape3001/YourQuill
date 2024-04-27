////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>
#include <mithril/preamble.hpp>
#include <mithril/enum/SizeDesc.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&, Image);
    WebHtml&    operator<<(WebHtml&, Dev<Image>);
    WebHtml&    operator<<(WebHtml&, DevID<Image>);
    WebHtml&    operator<<(WebHtml&, const Thumbnail&);
    WebHtml&    operator<<(WebHtml&, SizeDesc);
    
    void dev_table(WebHtml&, const ImageVector&);
    void dev_title(WebHtml&, Image, std::string_view extra=std::string_view());
}

