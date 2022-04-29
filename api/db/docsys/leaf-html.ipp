////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml& h, Leaf l)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(l), h.context().session.icon_size);
            
            // start the url (later)
            if(th)
                h << th << " ";
            h << cdb::label(l);
            //  end the url(later)
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, Dev<Leaf> v)
        {
            h << "<a href=\"/dev/leaf?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << cdb::label(v.data);
            h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<Leaf> v)
        {
            if(v.data)
                h << "<a href=\"/dev/leaf?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }
    
        void        dev_table(WebHtml&h, const std::vector<Leaf>&leafs)
        {
            auto t=h.table();
            h << "<tr><th><ID></th><th>key</th><th>Title</th></tr>\n";
            for(Leaf l : leafs){
                auto    i   = cdb::info(l);
                h << "<tr><td>" << dev_id(l) << "</td><td><a href=\"/leaf?id=" << l.id << "\">"
                    << i.key << "<a></td><td>" << i.title << "</td></tr>\n";
            }
        }
    }
}
