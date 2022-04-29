////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml&h, Dev<const PropertyInfo*> v)
        {
            if(v.data){
                h << v.data->name();
                //h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->name() << "</a>";
            } else {
                h << "(null property-info)";
            }
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<const PropertyInfo*> v)
        {
            if(v.data){
                h << v.data->id();
                //h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->id() << "</a>";
            } else {
                h << "(null property-info)";
            }
            return h;
        }
        
        void    dev_table(WebHtml&h, const std::vector<const PropertyInfo*>& objects)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Name</th><th>Type</th><th>State</th><th>Static</th><th>Description</th></tr>\n";
            for(const PropertyInfo* ti : objects){
                h << "<tr><td>"  << dev_id(ti) 
                  << "</td><td>" << dev(ti) 
                  << "</td><td>" << dev(&ti->type()) 
                  << "</td><td>" << ti->is_state() 
                  << "</td><td>" << ti->is_static() 
                  << "</td><td>" << ti->description() 
                  << "</td></tr>\n";
            }
        }
    }
}
