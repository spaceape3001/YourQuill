////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/db/Html.hpp>

namespace yq {
    struct Folder;
    namespace html {
        //WebHtml&    operator<<(WebHtml&, Folder);
        WebHtml&    operator<<(WebHtml&, Dev<Folder>);
        WebHtml&    operator<<(WebHtml&, DevID<Folder>);
        void        dev_table(WebHtml&, const std::vector<Folder>&);
    }
}


