////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/db/Html.hpp>
#include <yq/enum/DataRole.hpp>

namespace yq {
    struct Root;
    
    namespace html {
        WebHtml&    operator<<(WebHtml&, const Root*);
        WebHtml&    operator<<(WebHtml&, Dev<const Root*>);
        WebHtml&    operator<<(WebHtml&, DevID<const Root*>);
        void        dev_table(WebHtml&, const std::vector<const Root*>&);
        
        struct ControlRoot {
        };

        struct IRoot {
            std::string_view    msg = "Change Root:";
            const Root*         root = nullptr;
            DataRole            role;
        };
        
        WebHtml&    operator<<(WebHtml&, const ControlRoot&);
        WebHtml&    operator<<(WebHtml&, const IRoot&);
        
    }
}


