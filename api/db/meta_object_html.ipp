////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/ObjectInfo.hpp>


namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml&h, Dev<const ObjectInfo*> v)
        {
            if(v.data){
                h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->name() << "</a>";
            } else {
                h << "(null object-info)";
            }
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<const ObjectInfo*> v)
        {
            if(v.data){
                h << "<a href=\"/dev/meta/object?id=" << v.data->id() << "\">" << v.data->id() << "</a>";
            } else {
                h << "(null object-info)"; 
            }
            return h;
        }
        
        void    dev_table(WebHtml&h, const std::vector<const ObjectInfo*>& objects)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Name</th><th>Base</th><th>Size</th><th>Description</th></tr>\n";
            for(const ObjectInfo* ti : objects){
                h << "<tr><td>"  << dev_id(ti) 
                  << "</td><td>" << ti->name() 
                  << "</td><td>" << dev(ti->base()) 
                  << "</td><td>" << ti->size() 
                  << "</td><td>" << ti->description() 
                  << "</td></tr>\n";
            }
        }
    }
}
