////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/attribute/Attribute.hpp>
#include <mithril/attribute/AttributeCDB.hpp>
#include <mithril/attribute/AttributeSearch.hpp>

#include <mithril/document/Document.hpp>
#include <mithril/document/DocumentArg.hpp>
#include <mithril/document/DocumentCDB.hpp>

namespace yq::mithril {
    bool            search(WebSearcher<Attribute>&ws, document_t)
    {
        return search(ws, "document",
            [](std::string_view s) -> Document
            {
                return arg::document(s);
            },
            [](Document d) -> auto
            {
                return cdb::all_attributes(d, Sorted::YES);
            },
            [](Document d, Attribute a) -> bool
            {
                return cdb::document(a) != d;
            }
        );
    }
    

    AttributeVector  search(WebContext&ctx, attribute_t)
    {
        WebSearcher<Attribute>   ws(ctx);
        search(ws, DOCUMENT);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_attributes(Sorted::YES);
        return ws.data;
    }
}
