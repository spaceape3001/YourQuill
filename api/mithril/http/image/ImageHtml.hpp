////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/http/Html.hpp>
#include <mithril/kernel/enum/SizeDesc.hpp>

namespace yq::mithril {
    struct Image;
    struct Thumbnail;

    namespace html {

    
        WebHtml&    operator<<(WebHtml&, Image);
        WebHtml&    operator<<(WebHtml&, Dev<Image>);
        WebHtml&    operator<<(WebHtml&, DevID<Image>);
        WebHtml&    operator<<(WebHtml&, const Thumbnail&);
        WebHtml&    operator<<(WebHtml&, SizeDesc);
        void        dev_table(WebHtml&, const std::vector<Image>&);
    }
}


