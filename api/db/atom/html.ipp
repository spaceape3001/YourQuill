////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    namespace html {
        #if 0
        WebHtml&    operator<<(WebHtml&h, Atom v)
        {
        }
        
        WebHtml&    operator<<(WebHtml&h, Dev<Atom> v)
        {
            if(v.data)
                h << "<a href=\"/dev/atom?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }
        #endif
        
        WebHtml&    operator<<(WebHtml&h, DevID<Atom> v)
        {
            if(v.data)
                h << "<a href=\"/dev/atom?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }
        
        //void        dev_table(WebHtml&h, const std::vector<Atom>& table);
    }
}
