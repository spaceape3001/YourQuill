////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/html_common.hpp>

namespace yq {
    struct Category;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Category);
        WebHtml&    operator<<(WebHtml&, Dev<Category>);
        WebHtml&    operator<<(WebHtml&, DevID<Category>);
        void        dev_table(WebHtml&, const std::vector<Category>&);
    }
}


