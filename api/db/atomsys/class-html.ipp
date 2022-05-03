////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml&h, Class v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v), h.context().session.icon_size);
            if(v)
                h << "<a href=\"/class?key=" << cdb::key(v) << "\">";
            //  start the url (later)
            if(th)
                h << th << " ";
            h << cdb::label(v);
            if(v)
                h << "</a>";
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
        
        WebHtml&    operator<<(WebHtml&h, Plural<Class>v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v.data), h.context().session.icon_size);
            if(v.data)
                h << "<a href=\"/atoms?class=" << cdb::key(v.data) << "\">";
            //  start the url (later)
            if(th)
                h << th << " ";
            h << cdb::label(v.data);
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
