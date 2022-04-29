////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

//#include "cdb.hpp"
//#include "html.hpp"
//#include <db/document/cdb.hpp>
//#include <db/image/cdb.hpp>
//#include <db/image/struct.hpp>
//#include <db/thumbnail/html.hpp>
//#include <db/thumbnail/struct.hpp>

namespace yq {
    namespace html {

        WebHtml&    operator<<(WebHtml&h, User v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v), h.context().session.icon_size);
            
            //  start the url (later)
            if(th)
                h << th << " ";
            
            h << cdb::label(v);
            //  end the url (later)
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, Dev<User> v)
        {
            std::string n       = cdb::name(v.data);
            if(n.empty())
                n   = "(no-name)";
            if(v.data)
                h << "<a href=\"/dev/user?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << n;
            if(v.data)
                h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, DevID<User> v)
        {
            if(v.data)
                h << "<a href=\"/dev/user?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        void    dev_table(WebHtml& h, const std::vector<User>&users)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Owner</th><th>Admin</th><th>Writer</th><th>Reader</th><th>Name</th><th>Doc</th><th>Brief</th></tr>\n";
            for(User t : users){
                auto i  = cdb::info(t);
                h << "<tr><td>" << dev_id(t) << "</td><td>" << i.key << "</td><td>" 
                    << i.is_owner << "</td><td>"
                    << i.is_admin << "</td><td>"
                    << i.is_writer << "</td><td>"
                    << i.is_reader << "</td><td>"
                    <<  i.name << "</td><td>" << dev(i.doc) << "</td><td>" << i.brief << "</td></tr>\n";
            }
        }    
    }
}
