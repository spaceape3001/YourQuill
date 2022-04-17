////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    namespace html {
        //WebHtml&    operator<<(WebHtml&, Tag);
    
        WebHtml&    operator<<(WebHtml&h, Dev<Tag> v)
        {
            std::string n       = cdb::name(v.data);
            if(n.empty())
                n   = "(no-name)";
            if(v.data)
                h << "<a href=\"/dev/tag?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << n;
            if(v.data)
                h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, DevID<Tag> v)
        {
            if(v.data)
                h << "<a href=\"/dev/tag?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        void    dev_table(WebHtml& h, const std::vector<Tag>&tags)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Doc</th><th>Leaf</th><th>Brief</th></tr>\n";
            for(Tag t : tags){
                auto i  = cdb::info(t);
                h << "<tr><td>" << dev_id(t) << "</td><td>" << i.key << "</td><td>" << i.name
                    << "</td><td>" << dev(i.doc) << "</td><td>" << /* dev(cdb::leaf(t)) << */ "</td><td>" << i.brief << "</td></tr>\n";
            }
        }

    }
}
