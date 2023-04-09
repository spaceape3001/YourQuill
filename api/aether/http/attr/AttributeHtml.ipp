////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AttributeHtml.hpp"
#include <aether/kernel/attr/AttributeCDB.hpp>

namespace yq {
    namespace html {
        void    dev_table(WebHtml&h, const std::vector<Attribute>&table)
        {
            auto ta = h.table();
            ta << "<TR><TH>ID</TH><TH>Key</TH><TH>Value</TH></TR>\n";
            for(Attribute a : table){
                auto i = cdb::info(a);
                ta << "<tr><td>" << a.id << "</td><td>" << i.key << "</td><td>";
                html_escape_write(h, i.value);
                ta << "</td></tr>\n";
            }
        }
    }
}
