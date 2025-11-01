////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/html/Html.hpp>
#include <yq/mithril/preamble.hpp>

namespace yq::mithril::html {
    //WebHtml&    operator<<(WebHtml&, Tag);
    WebHtml&    operator<<(WebHtml&, Dev<Tag>);
    WebHtml&    operator<<(WebHtml&, DevID<Tag>);
    WebHtml&    operator<<(WebHtml&, const TagSet&);
    WebHtml&    operator<<(WebHtml&, const TagVector&);
    WebHtml&    operator<<(WebHtml&, const Dev<TagSet>&);
    WebHtml&    operator<<(WebHtml&, const Dev<TagVector>&);
    
    void admin_table(WebHtml&, const TagVector&);
    void dev_table(WebHtml&, const TagVector&);
    void dev_title(WebHtml&, Tag, std::string_view extra=std::string_view());
    void new_tag_control(WebHtml&, std::string_view npath);
}

