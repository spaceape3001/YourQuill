////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/db/Html.hpp>

namespace yq {
    struct Class;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Class);
        WebHtml&    operator<<(WebHtml&, Dev<Class>);
        WebHtml&    operator<<(WebHtml&, DevID<Class>);
        WebHtml&    operator<<(WebHtml&, const Edit<Class>& );
        WebHtml&    operator<<(WebHtml&, const Plural<Class>&);
        void        dev_table(WebHtml&, const std::vector<Class>&);
    }
}


