////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cdb.hpp"
#include "html.hpp"
#include <db/document/html.hpp>
#include <db/image/cdb.hpp>
#include <db/image/struct.hpp>
#include <db/thumbnail/html.hpp>
#include <db/thumbnail/struct.hpp>

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml& h, Category v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v), h.context().session.icon_size);
            
            //  start the url (later)
            if(th)
                h << th << " ";
            
            h << cdb::label(v);
            //  end the url (later)
            return h;
        }
    
        WebHtml&    operator<<(WebHtml&h, Dev<Category> v)
        {
            std::string n       = cdb::label(v.data);
            if(n.empty())
                n   = "(no-name)";
            if(v.data)
                h << "<a href=\"/dev/category?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << n;
            if(v.data)
                h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, DevID<Category> v)
        {
            if(v.data)
                h << "<a href=\"/dev/category?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        void    dev_table(WebHtml& h, const std::vector<Category>&categorys)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Doc</th><th>Brief</th></tr>\n";
            for(Category t : categorys){
                auto i  = cdb::info(t);
                h << "<tr><td>" << dev_id(t) << "</td><td>" << i.key << "</td><td>" << i.name
                    << "</td><td>" << dev(i.doc) << "</td><td>" << i.brief << "</td></tr>\n";
            }
        }

    }
}
