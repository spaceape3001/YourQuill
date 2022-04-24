////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "html_common.hpp"

namespace yq {
    
    class WebHtml;
    class PropertyInfo;

    namespace html {
        WebHtml&    operator<<(WebHtml&, DevID<const PropertyInfo*>);
        WebHtml&    operator<<(WebHtml&, Dev<const PropertyInfo*>);
        void    dev_table(WebHtml&, const std::vector<const PropertyInfo*>&);
        
    }
}
