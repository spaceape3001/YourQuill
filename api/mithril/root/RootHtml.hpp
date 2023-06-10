////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>
#include <mithril/enum/Access.hpp>
#include <mithril/enum/DataRole.hpp>

namespace yq::mithril {
    struct RootDir;
    
    namespace html {
        WebHtml&    operator<<(WebHtml&, const RootDir*);
        WebHtml&    operator<<(WebHtml&, Dev<const RootDir*>);
        WebHtml&    operator<<(WebHtml&, DevID<const RootDir*>);
        void        dev_table(WebHtml&, const std::vector<const RootDir*>&);
        
        struct ControlRoot {
        };

        struct IRoot {
            std::string_view    msg = "Change RootDir:";
            const RootDir*         root_dir = nullptr;
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

        inline IRoot iroot(const RootDir* rt, Access ac=Access()) 
        {
            IRoot ret;
            ret.root_dir    = rt;
            ret.access  = ac;
            return ret;
        }
        
        WebHtml&    operator<<(WebHtml&, const ControlRoot&);
        WebHtml&    operator<<(WebHtml&, const IRoot&);
        
    }
}


