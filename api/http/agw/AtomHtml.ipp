////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AtomHtml.hpp"
#include <kernel/agw/AtomCDB.hpp>

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml&h, agw::Atom v)
        {
            //  more TODO...
            h << cdb::label(v);
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, Dev<agw::Atom> v)
        {
            if(v.data)
                h << "<a href=\"/dev/atom?id=" << v.data.id << "\">";
            h << cdb::label(v.data);
            if(v.data)
                h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<agw::Atom> v)
        {
            if(v.data)
                h << "<a href=\"/dev/atom?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }
        
        void        dev_table(WebHtml&h, const agw::AtomVector& atoms)
        {
            auto t = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Brief</th></tr>\n";
            for(agw::Atom a : atoms){
                auto i = cdb::info(a);
                h << "<tr><td>" << dev_id(a) << "</td><td>" << i.key << "</td><td>" << i.brief << "</td></tr>\n";
            }
        }
    }
}
