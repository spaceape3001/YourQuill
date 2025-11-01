////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/doodle/Doodle.hpp>
#include <mithril/doodle/DoodleCDB.hpp>
#include <mithril/doodle/DoodleSearch.hpp>
#include <mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    DoodleVector  search(WebContext&ctx, doodle_k)
    {
        WebSearcher<Doodle>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_doodles(Sorted::YES);
        return ws.data;
    }
}
