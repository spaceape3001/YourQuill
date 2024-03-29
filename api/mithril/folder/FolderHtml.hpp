////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>

namespace yq::mithril {
    struct Folder;
    namespace html {
        //WebHtml&    operator<<(WebHtml&, Folder);
        WebHtml&    operator<<(WebHtml&, Dev<Folder>);
        WebHtml&    operator<<(WebHtml&, DevID<Folder>);
        WebHtml&    operator<<(WebHtml&, const Icon<Folder>&);
        void        dev_table(WebHtml&, const std::vector<Folder>&);
    }
}


