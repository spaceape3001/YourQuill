////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>
#include <mithril/class/Class.hpp>

namespace yq::mithril {
    struct Class;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Class);
        WebHtml&    operator<<(WebHtml&, Dev<Class>);
        WebHtml&    operator<<(WebHtml&, DevID<Class>);
        WebHtml&    operator<<(WebHtml&, const Edit<Class>& );
        WebHtml&    operator<<(WebHtml&, const std::set<Class>&);
        WebHtml&    operator<<(WebHtml&, const std::vector<Class>&);
        WebHtml&    operator<<(WebHtml&, const Dev<std::set<Class>>&);
        WebHtml&    operator<<(WebHtml&, const Dev<std::vector<Class>>&);
        //WebHtml&    operator<<(WebHtml&, const LinkKey<Class>& );
        //WebHtml&    operator<<(WebHtml&, const LinkLabel<Class>& );
        //WebHtml&    operator<<(WebHtml&, const LinkThumb<Class>& );
        WebHtml&    operator<<(WebHtml&, const Plural<Class>&);
        void        admin_table(WebHtml&, const std::vector<Class>&);
        void        new_class_control(WebHtml&, std::string_view npath);
        void        dev_table(WebHtml&, const std::vector<Class>&);
        void        dev_table(WebHtml&, const std::vector<Class::Rank>&, std::string_view rankName);
    }
}


