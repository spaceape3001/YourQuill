////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/field/Field.hpp>
#include <mithril/field/FieldCDB.hpp>
#include <mithril/field/FieldSearch.hpp>
#include <mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    FieldVector  search(WebContext&ctx, field_k)
    {
        WebSearcher<Field>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_fields(Sorted::YES);
        return ws.data;
    }
}
