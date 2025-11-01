////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/html/Html.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&, Category);
    WebHtml&    operator<<(WebHtml&, Dev<Category>);
    WebHtml&    operator<<(WebHtml&, DevID<Category>);
    
    void admin_table(WebHtml&, const CategoryVector&);
    void dev_table(WebHtml&, const CategoryVector&);
    void dev_title(WebHtml&, Category, std::string_view extra=std::string_view());
    void new_category_control(WebHtml&, std::string_view npath);
}


