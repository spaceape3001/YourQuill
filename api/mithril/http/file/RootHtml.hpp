////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/http/Html.hpp>
#include <mithril/kernel/enum/Access.hpp>
#include <mithril/kernel/enum/DataRole.hpp>

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
            Access              access;
        };
        
        inline IRoot iroot(DataRole r, Access ac=Access()) 
        {
            IRoot ret;
            ret.role    = r;
            ret.access  = ac;
            return ret;
        }

        inline IRoot iroot(const Root* rt, Access ac=Access()) 
        {
            IRoot ret;
            ret.root    = rt;
            ret.access  = ac;
            return ret;
        }
        
        WebHtml&    operator<<(WebHtml&, const ControlRoot&);
        WebHtml&    operator<<(WebHtml&, const IRoot&);
        
    }
}


