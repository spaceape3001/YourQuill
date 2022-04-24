////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    namespace html {
        #if 0
        WebHtml&    operator<<(WebHtml&h, Field v)
        {
        }
        
        WebHtml&    operator<<(WebHtml&h, Dev<Field> v)
        {
            if(v.data)
                h << "<a href=\"/dev/field?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }
        #endif
        
        WebHtml&    operator<<(WebHtml&h, DevID<Field> v)
        {
            if(v.data)
                h << "<a href=\"/dev/field?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }
        
        //void        dev_table(WebHtml&h, const std::vector<Field>& table);
    }
}
