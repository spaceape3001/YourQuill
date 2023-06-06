////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/attribute/AttributeHtml.hpp>
#include <mithril/attribute/AttributeCDB.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&h, Dev<Attribute> v)
    {
        if(v.data)
            h << "<a href=\"/dev/attribute?id=" << v.data.id << "\">";
        h << cdb::key(v.data);
        if(v.data)
            h << "</a>";
        return h;
    }

    WebHtml&    operator<<(WebHtml&h, DevID<Attribute> v)
    {
        if(v.data)
            h << "<a href=\"/dev/attribute?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }

    void    dev_table(WebHtml&h, const std::vector<Attribute>&table)
    {
        auto ta = h.table();
        ta << "<TR><TH>ID</TH><TH>Key</TH><TH>Value</TH></TR>\n";
        for(Attribute a : table){
            auto i = cdb::info(a);
            ta << "<tr><td>" << dev_id(a) << "</td><td>" << i.key << "</td><td>";
            html_escape_write(h, i.value);
            ta << "</td></tr>\n";
        }
    }
}
