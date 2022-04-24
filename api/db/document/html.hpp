////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/html_common.hpp>

namespace yq {
    struct Document;
    namespace html {
        //WebHtml&    operator<<(WebHtml&, Document);
        WebHtml&    operator<<(WebHtml&, Dev<Document>);
        WebHtml&    operator<<(WebHtml&, DevID<Document>);
        void        dev_table(WebHtml&, const std::vector<Document>&);
    }
}


