////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DirectoryHtml.hpp"
#include <kernel/file/DirectoryCDB.hpp>

namespace yq {

    namespace html {
        //WebHtml&    operator<<(WebHtml&, Directory);

        WebHtml&    operator<<(WebHtml&h, Dev<Directory> v)
        {
            std::string n       = cdb::name(v.data);
            if(n.empty())
                n   = "(no-name)";
            if(v.data)
                h << "<a href=\"/dev/directory?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << n;
            if(v.data)
                h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, DevID<Directory> v)
        {
            if(v.data)
                h << "<a href=\"/dev/directory?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        void    dev_table(WebHtml& h, const std::vector<Directory>& dirs)
        {
            auto _tab = h.table();
                h << "<tr><th>ID</th><th>Fragments</th><th>Children</th><th>Path</th></tr>\n";
            for(Directory d : dirs){
                h << "<tr><td>" << dev_id(d) << "</td><td>" 
                    << cdb::child_fragments_count(d) << "</td><td>" << cdb::child_directories_count(d) << "</td><td>" << cdb::path(d) << "</td></tr>\n";
            }
        }


    }
}
