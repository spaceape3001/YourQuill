////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <aether/http/Html.hpp>
#include <vector>

namespace yq {
    struct Category;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Category);
        WebHtml&    operator<<(WebHtml&, Dev<Category>);
        WebHtml&    operator<<(WebHtml&, DevID<Category>);
        void        admin_table(WebHtml&, const std::vector<Category>&);
        void        dev_table(WebHtml&, const std::vector<Category>&);
        void        new_category_control(WebHtml&, std::string_view npath);
    }
}


