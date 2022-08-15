////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <http/Html.hpp>

namespace yq {
    struct Class;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Class);
        WebHtml&    operator<<(WebHtml&, Dev<Class>);
        WebHtml&    operator<<(WebHtml&, DevID<Class>);
        WebHtml&    operator<<(WebHtml&, const Edit<Class>& );
        //WebHtml&    operator<<(WebHtml&, const LinkKey<Class>& );
        //WebHtml&    operator<<(WebHtml&, const LinkLabel<Class>& );
        //WebHtml&    operator<<(WebHtml&, const LinkThumb<Class>& );
        WebHtml&    operator<<(WebHtml&, const Plural<Class>&);
        void        admin_table(WebHtml&, const std::vector<Class>&);
        void        new_class_control(WebHtml&, std::string_view npath);
        void        dev_table(WebHtml&, const std::vector<Class>&);
    }
}


