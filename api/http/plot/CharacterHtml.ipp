////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CharacterHtml.hpp"
#include <kernel/atom/AtomCDB.hpp>
#include <kernel/plot/CharacterCDB.hpp>

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml&h, Character v)
        {
            //  more TODO...
            h << cdb::label(cdb::atom(v));  // FIXME/CRUTCH
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, Dev<Character> v)
        {
            if(v.data)
                h << "<a href=\"/dev/character?id=" << v.data.id << "\">";
            h << cdb::label(cdb::atom(v.data));  // FIXME/CRUTCH
            if(v.data)
                h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<Character> v)
        {
            if(v.data)
                h << "<a href=\"/dev/character?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }
        
        void        dev_table(WebHtml&h, const std::vector<Character>& chars)
        {
            auto t = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Brief</th></tr>\n";
            for(Character ch : chars){
                Atom a = cdb::atom(ch);  // temporary hack
                auto i = cdb::info(a);
                h << "<tr><td>" << dev_id(a) << "</td><td>" << i.key << "</td><td>" << i.brief << "</td></tr>\n";
            }
        }
    }
}
