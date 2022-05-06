////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/db/Html.hpp>
#include <vector>

namespace yq {
    
    class WebHtml;
    class TypeInfo;

    namespace html {
        WebHtml&    operator<<(WebHtml&, DevID<const TypeInfo*>);
        WebHtml&    operator<<(WebHtml&, Dev<const TypeInfo*>);
        void    dev_table(WebHtml&, const std::vector<const TypeInfo*>&);
        
    }
}
