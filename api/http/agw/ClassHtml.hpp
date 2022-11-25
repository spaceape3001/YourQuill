////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <http/Html.hpp>
#include <kernel/agw/Class.hpp>

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml&, agw::Class);
        WebHtml&    operator<<(WebHtml&, Dev<agw::Class>);
        WebHtml&    operator<<(WebHtml&, DevID<agw::Class>);
        WebHtml&    operator<<(WebHtml&, const Edit<agw::Class>& );
        //WebHtml&    operator<<(WebHtml&, const LinkKey<agw::Class>& );
        //WebHtml&    operator<<(WebHtml&, const LinkLabel<agw::Class>& );
        //WebHtml&    operator<<(WebHtml&, const LinkThumb<agw::Class>& );
        WebHtml&    operator<<(WebHtml&, const Plural<agw::Class>&);
        void        admin_table(WebHtml&, const agw::ClassVector&);
        void        new_class_control(WebHtml&, std::string_view npath);
        void        dev_table(WebHtml&, const agw::ClassVector&);
        void        dev_table(WebHtml&, const std::vector<agw::Class::Rank>&, std::string_view rankName);
    }
}


