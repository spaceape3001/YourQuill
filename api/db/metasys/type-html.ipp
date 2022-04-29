////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml&h, Dev<const TypeInfo*> v)
        {
            if(v.data){
                h << "<a href=\"/dev/meta/type?id=" << v.data->id() << "\">" << v.data->name() << "</a>";
            } else {
                h << "(null type info)";
            }
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<const TypeInfo*> v)
        {
            if(v.data){
                h << "<a href=\"/dev/meta/type?id=" << v.data->id() << "\">" << v.data->id() << "</a>";
            } else {
                h << "(null type info)";
            }
            return h;
        }
        
        void    dev_table(WebHtml&h, const std::vector<const TypeInfo*>& types)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Name</th><th>Size</th><th>Description</th></tr>\n";
            for(const TypeInfo* ti : types){
                if(!ti)
                    continue;
                h << "<tr><td>"  << dev_id(ti) 
                  << "</td><td>" << dev(ti) 
                  << "</td><td>" << ti->size() 
                  << "</td><td>" << ti->description() 
                  << "</td></tr>\n";
            }
        }
    }
}
