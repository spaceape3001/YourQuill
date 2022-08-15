////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DocumentHtml.hpp"
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/image/ImageCDB.hpp>

namespace yq {

    namespace html {
        //WebHtml&    operator<<(WebHtml&, Document);

        WebHtml&    operator<<(WebHtml&h, Dev<Document> v)
        {
            std::string n       = cdb::name(v.data);
            if(n.empty())
                n   = "(no-name)";
            if(v.data)
                h << "<a href=\"/dev/document?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << n;
            if(v.data)
                h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, DevID<Document> v)
        {
            if(v.data)
                h << "<a href=\"/dev/document?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, const Icon<Document>& v)
        {
            std::string a   = v.alt;
            if(a.empty())
                a   = cdb::key(v.src);
            
            Image   i   = cdb::icon(v.src);
            if(i){
                h << cdb::thumbnail(i, a);
            } else {
                Icon<std::string>   fwd;
                fwd.src = "/img/document.svg";
                fwd.style = v.style;
                fwd.alt = std::move(a);
                h << fwd;
            }
            return h;
        }

        void    dev_table(WebHtml& h, const std::vector<Document>& documents)
        {
            auto _tab = h.table();
            h << "<tr><th>ID</th><th>Fragments</th><th>Key</th><th>Name</th><th>Suffix</th>\n";
            for(Document a : documents){
                auto i = cdb::info(a);
                h << "<tr><td>" << dev_id(a)  << "</td><td>" << cdb::fragments_count(a) 
                  << "</td><td>" << i.key << "</td><td>" << i.name << "</td><td>" << i.suffix << "</td></tr>\n";
            }
        }
    }
}
