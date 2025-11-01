////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/preamble.hpp>
#include <yq/mithril/html/Html.hpp>
#include <yq/mithril/class/Class.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&, Field);
    WebHtml&    operator<<(WebHtml&, Dev<Field>);
    WebHtml&    operator<<(WebHtml&, const std::set<Field>&);
    WebHtml&    operator<<(WebHtml&, const FieldVector&);
    WebHtml&    operator<<(WebHtml&, const Dev<std::set<Field>>&);
    WebHtml&    operator<<(WebHtml&, const Dev<FieldVector>&);
    WebHtml&    operator<<(WebHtml&, DevID<Field>);

    void admin_table(WebHtml&, const FieldVector&fields);
    void dev_table(WebHtml&, const FieldVector& fields);
    void dev_title(WebHtml&, Field, std::string_view extra=std::string_view());
    void new_field_control(WebHtml&, std::string_view npath, Class c={});
}


