////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/db/Html.hpp>

namespace yq {
    struct Image;
    struct Thumbnail;

    namespace html {
    
        WebHtml&    operator<<(WebHtml&, Image);
        WebHtml&    operator<<(WebHtml&, Dev<Image>);
        WebHtml&    operator<<(WebHtml&, DevID<Image>);
        WebHtml&    operator<<(WebHtml&, const Thumbnail&);
        void        dev_table(WebHtml&, const std::vector<Image>&);
    }
}


