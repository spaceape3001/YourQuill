////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/db/Html.hpp>

namespace yq {
    struct Document;
    namespace html {
        //WebHtml&    operator<<(WebHtml&, Document);
        WebHtml&    operator<<(WebHtml&, Dev<Document>);
        WebHtml&    operator<<(WebHtml&, DevID<Document>);
        WebHtml&    operator<<(WebHtml&, const Icon<Document>&);
        void        dev_table(WebHtml&, const std::vector<Document>&);
    }
}


