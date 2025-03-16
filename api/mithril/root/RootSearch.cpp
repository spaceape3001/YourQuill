////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include <mithril/root/Root.hpp>
#include <mithril/root/RootArg.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/root/RootCDB.hpp>
#include <mithril/root/RootSearch.hpp>

#include <mithril/wksp/Workspace.hpp>
#include <mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    RootDirVector  search(WebContext&ctx, root_k)
    {
        WebSearcher<const RootDir*>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = wksp::root_dirs();
        return ws.data;
    }
}
