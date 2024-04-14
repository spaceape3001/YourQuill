////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>
#include <vector>

namespace yq::mithril {
    struct Tag;
    namespace html {
        //WebHtml&    operator<<(WebHtml&, Tag);
        WebHtml&    operator<<(WebHtml&, Dev<Tag>);
        WebHtml&    operator<<(WebHtml&, DevID<Tag>);
        WebHtml&    operator<<(WebHtml&, const std::set<Tag>&);
        WebHtml&    operator<<(WebHtml&, const std::vector<Tag>&);
        WebHtml&    operator<<(WebHtml&, const Dev<std::set<Tag>>&);
        WebHtml&    operator<<(WebHtml&, const Dev<std::vector<Tag>>&);
        void        admin_table(WebHtml&, const std::vector<Tag>&);
        void        dev_table(WebHtml&, const std::vector<Tag>&);

        void        new_tag_control(WebHtml&, std::string_view npath);
    }
}


