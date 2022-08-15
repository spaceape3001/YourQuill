////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <http/Html.hpp>
#include <vector>

namespace yq {
    struct Tag;
    namespace html {
        //WebHtml&    operator<<(WebHtml&, Tag);
        WebHtml&    operator<<(WebHtml&, Dev<Tag>);
        WebHtml&    operator<<(WebHtml&, DevID<Tag>);
        void        admin_table(WebHtml&, const std::vector<Tag>&);
        void        dev_table(WebHtml&, const std::vector<Tag>&);

        void        new_tag_control(WebHtml&, std::string_view npath);
    }
}


