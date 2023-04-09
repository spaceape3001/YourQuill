////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <aether/http/Html.hpp>

namespace yq {
    struct Directory;
    namespace html {
        //WebHtml&    operator<<(WebHtml&, Directory);
        WebHtml&    operator<<(WebHtml&, Dev<Directory>);
        WebHtml&    operator<<(WebHtml&, DevID<Directory>);
        void        dev_table(WebHtml&, const std::vector<Directory>&);
    }
}


