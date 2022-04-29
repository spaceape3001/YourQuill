////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml&h, Field v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v), h.context().session.icon_size);
            
            //  start the url (later)
            if(th)
                h << th << " ";
            
            h << cdb::label(v);
            //  end the url (later)
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, Dev<Field> v)
        {
            if(v.data)
                h << "<a href=\"/dev/field?id=" << v.data.id << "\">";
            h << cdb::label(v.data);
            if(v.data)
                h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<Field> v)
        {
            if(v.data)
                h << "<a href=\"/dev/field?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }
        
        void        dev_table(WebHtml&h, const std::vector<Field>& fields)
        {
            auto t = h.table();
            h << "<tr><th>ID</th><th>Class</th><th>Key</th><th>Name</th><th>Brief</th>\n";
            for(Field f : fields){
                auto i = cdb::info(f);
                h << "<tr><td>" << dev_id(f) << "</td><td>" << dev(i.class_) << "</td><td>" 
                    << i.key <<"</td><td>" << i.name << "</td><td>"
                    << i.brief <<"</td></tr>\n";
            }
        }
    }
}
