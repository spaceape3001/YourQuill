////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/core/html_common.hpp>
#include <vector>

namespace yq {
    
    class WebHtml;
    class ObjectInfo;

    namespace html {
        WebHtml&    operator<<(WebHtml&, DevID<const ObjectInfo*>);
        WebHtml&    operator<<(WebHtml&, Dev<const ObjectInfo*>);
        void    dev_table(WebHtml&, const std::vector<const ObjectInfo*>&);
        
    }
}