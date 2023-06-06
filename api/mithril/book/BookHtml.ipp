////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "BookHtml.hpp"
#include <mithril/atom/AtomCDB.hpp>
#include <mithril/atom/AtomHtml.hpp>
#include <mithril/book/BookCDB.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&h, Book v)
    {
        //  more TODO...
        h << cdb::label(cdb::atom(v));  // FIXME/CRUTCH
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, Dev<Book> v)
    {
        if(v.data)
            h << "<a href=\"/dev/book?id=" << v.data.id << "\">";
        h << cdb::label(cdb::atom(v.data));  // FIXME/CRUTCH
        if(v.data)
            h << "</a>";
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, DevID<Book> v)
    {
        if(v.data)
            h << "<a href=\"/dev/book?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }
    
    void        dev_table(WebHtml&h, const std::vector<Book>& books)
    {
        auto t = h.table();
        h << "<tr><th>ID</th><th>Key</th><th>Brief</th></tr>\n";
        for(Book bk : books){
            Atom a = cdb::atom(bk);  // temporary hack
            auto i = cdb::info(a);
            h << "<tr><td>" << dev_id(a) << "</td><td>" << i.key << "</td><td>" << i.brief << "</td></tr>\n";
        }
    }
}
