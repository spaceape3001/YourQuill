////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>
#include <mithril/class/Class.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&, Class);
    WebHtml&    operator<<(WebHtml&, Dev<Class>);
    WebHtml&    operator<<(WebHtml&, DevID<Class>);
    WebHtml&    operator<<(WebHtml&, const Edit<Class>& );
    WebHtml&    operator<<(WebHtml&, const ClassSet&);
    WebHtml&    operator<<(WebHtml&, const ClassVector&);
    WebHtml&    operator<<(WebHtml&, const Dev<ClassSet>&);
    WebHtml&    operator<<(WebHtml&, const Dev<ClassVector>&);
    //WebHtml&    operator<<(WebHtml&, const LinkKey<Class>& );
    //WebHtml&    operator<<(WebHtml&, const LinkLabel<Class>& );
    //WebHtml&    operator<<(WebHtml&, const LinkThumb<Class>& );
    WebHtml&    operator<<(WebHtml&, const Plural<Class>&);

    void admin_table(WebHtml&, const ClassVector& classes);
    void dev_table(WebHtml&, const ClassVector& classes);
    void dev_table(WebHtml&, const std::vector<Class::Rank>&data, std::string_view rankName);
    void dev_title(WebHtml&, Class, std::string_view extra=std::string_view());
    void new_class_control(WebHtml&, std::string_view npath);
}

