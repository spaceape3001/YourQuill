////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/html/Html.hpp>
#include <yq/mithril/preamble.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&, User);
    WebHtml&    operator<<(WebHtml&, Dev<User>);
    WebHtml&    operator<<(WebHtml&, DevID<User>);
    
    void admin_table(WebHtml&, const std::vector<User>&);
    void dev_table(WebHtml&, const std::vector<User>&);
    void dev_title(WebHtml&, User, std::string_view extra=std::string_view());
    void new_user_control(WebHtml&, std::string_view npath);
}

