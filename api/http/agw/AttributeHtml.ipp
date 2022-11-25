////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AttributeHtml.hpp"
#include <kernel/agw/AttributeCDB.hpp>

namespace yq {
    namespace html {
        void    dev_table(WebHtml&h, const agw::AttributeVector &table)
        {
            auto ta = h.table();
            ta << "<TR><TH>ID</TH><TH>Key</TH><TH>Value</TH></TR>\n";
            for(agw::Attribute a : table){
                auto i = cdb::info(a);
                ta << "<tr><td>" << a.id << "</td><td>" << i.key << "</td><td>";
                html_escape_write(h, i.value);
                ta << "</td></tr>\n";
            }
        }
    }
}
