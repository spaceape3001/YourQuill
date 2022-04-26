////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cdb.hpp"
#include "html.hpp"
#include <db/image/cdb.hpp>
#include <db/image/struct.hpp>
#include <db/thumbnail/html.hpp>
#include <db/thumbnail/struct.hpp>

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml&h, Class v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v), h.context().session.icon_size);
            
            //  start the url (later)
            if(th)
                h << th << " ";
            
            h << cdb::label(v);
            //  end the url (later)
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, Dev<Class> v)
        {
            if(v.data)
                h << "<a href=\"/dev/class?id=" << v.data.id << "\">";
            h << cdb::label(v.data);
            if(v.data)
                h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<Class> v)
        {
            if(v.data)
                h << "<a href=\"/dev/class?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }
        
        void        dev_table(WebHtml&h, const std::vector<Class>& classes)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Doc</th><th>Brief</th></tr>\n";
            for(Class c : classes){
                auto i = cdb::info(c);
                h << "<tr><td>" << dev_id(c) << "</td><td>" << i.key << "</td><td>" << i.name << "</td><td>" 
                    << dev(i.doc) << "</td><td>" << i.brief << "</td></tr>\n";
            }
        }
        
    }
}
