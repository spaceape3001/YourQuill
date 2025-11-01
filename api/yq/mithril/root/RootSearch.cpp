////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include <yq/mithril/root/Root.hpp>
#include <yq/mithril/root/RootArg.hpp>
#include <yq/mithril/root/RootDir.hpp>
#include <yq/mithril/root/RootCDB.hpp>
#include <yq/mithril/root/RootSearch.hpp>

#include <yq/mithril/wksp/Workspace.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

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
