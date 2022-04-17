////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    namespace html {

        WebHtml&    operator<<(WebHtml&h, Dev<const Root*> v)
        {
            if(!v.data){
                h << "(null root)";
                return h;
            }
            
            h << "<a href=\"/dev/root?id=" << v.data->id << "\">[" << v.data->id << "] " << v.data->key << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml& h, DevID<const Root*> v)
        {
            if(!v.data){
                h << "(null root)";
                return h;
            }
            
            h << "<a href=\"/dev/root?id=" << v.data->id << "\">" << v.data->id << "</a>";
            return h;
        }

        void    dev_table(WebHtml&h, std::vector<const Root*> roots)
        {
            auto _tab = h.table();
            h << "<tr><th><ID></th><th>Key</th><th>Name</th><th>Path</th></tr>\n";
            for(const Root* r : roots){
                if(!r)
                    continue;
                h << "<tr><td>" << dev_id(r) <<  "</td><td>" << r->key << "</td><td>" << r->name << "</td><td>" << r->path << "</td></tr>\n";
            }
        }

    }
}
