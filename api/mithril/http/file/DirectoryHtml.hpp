////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/http/Html.hpp>

namespace yq::mithril {
    struct Directory;
    namespace html {
        //WebHtml&    operator<<(WebHtml&, Directory);
        WebHtml&    operator<<(WebHtml&, Dev<Directory>);
        WebHtml&    operator<<(WebHtml&, DevID<Directory>);
        void        dev_table(WebHtml&, const std::vector<Directory>&);
    }
}


