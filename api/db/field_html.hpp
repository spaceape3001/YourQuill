////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "html_common.hpp"
#include "field.hpp"

namespace yq {
    namespace html {
        //WebHtml&    operator<<(WebHtml&, Field);
        //WebHtml&    operator<<(WebHtml&, Dev<Field>);
        WebHtml&    operator<<(WebHtml&, DevID<Field>);
        //void        dev_table(WebHtml&, const std::vector<Field>&);
    }
}


