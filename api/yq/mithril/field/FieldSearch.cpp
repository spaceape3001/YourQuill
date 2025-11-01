////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/field/Field.hpp>
#include <yq/mithril/field/FieldCDB.hpp>
#include <yq/mithril/field/FieldSearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

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
