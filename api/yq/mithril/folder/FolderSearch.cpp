////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/folder/Folder.hpp>
#include <mithril/folder/FolderArg.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/folder/FolderSearch.hpp>
#include <mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    bool          search(WebSearcher<Folder>&ws, parent_k)
    {
        return search(ws, "parent",
            [](std::string_view s) -> Folder
            {
                return arg::folder(s);
            },
            [](Folder p) -> auto
            {
                return cdb::child_folders(p, Sorted::YES);
            },
            [](Folder p, Folder c) -> bool
            {
                return cdb::parent(c) != p;
            }
        );
    }

    FolderVector  search(WebContext& ctx, folder_k)
    {
        WebSearcher<Folder>   ws(ctx);
        search(ws, PARENT);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_folders(Sorted::YES);
        return ws.data;
    }
}
