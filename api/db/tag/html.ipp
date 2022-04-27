////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cdb.hpp"
#include "html.hpp"
#include <db/document/cdb.hpp>
#include <db/image/cdb.hpp>
#include <db/image/struct.hpp>
#include <db/thumbnail/html.hpp>
#include <db/thumbnail/struct.hpp>

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml&h, Tag t)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(t), h.context().session.icon_size);
            
            //  start the url (later)
            if(th)
                h << th << " ";
            
            h << cdb::label(t);
            //  end the url (later)
            return h;
        }
    
        WebHtml&    operator<<(WebHtml&h, Dev<Tag> v)
        {
            if(v.data)
                h << "<a href=\"/dev/tag?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << cdb::label(v.data);
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