////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/core/html_common.hpp>
#include <vector>

namespace yq {
    struct Category;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Category);
        WebHtml&    operator<<(WebHtml&, Dev<Category>);
        WebHtml&    operator<<(WebHtml&, DevID<Category>);
        void        dev_table(WebHtml&, const std::vector<Category>&);
    }
}


