////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/doodle/Doodle.hpp>
#include <yq/mithril/doodle/DoodleCDB.hpp>
#include <yq/mithril/doodle/DoodleSearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

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
