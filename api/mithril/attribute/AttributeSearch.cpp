////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/attribute/Attribute.hpp>
#include <mithril/attribute/AttributeCDB.hpp>
#include <mithril/attribute/AttributeSearch.hpp>

#include <mithril/document/Document.hpp>
#include <mithril/document/DocumentArg.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    bool            search(WebSearcher<Attribute>&ws, document_k)
    {
        return search(ws, "document",
            [](std::string_view s) -> Document
            {
                return arg::document(s);
            },
            [](Document d) -> auto
            {
                return cdb::all_attributes(d);
            },
            [](Document d, Attribute a) -> bool
            {
                return cdb::document(a) != d;
            }
        );
    }
    

    AttributeVector  search(WebContext&ctx, attribute_k)
    {
        WebSearcher<Attribute>   ws(ctx);
        search(ws, DOCUMENT);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_attributes();
        return ws.data;
    }
}
