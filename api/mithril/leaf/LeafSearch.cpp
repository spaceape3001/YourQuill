////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/folder/Folder.hpp>
#include <mithril/folder/FolderArg.hpp>
#include <mithril/leaf/Leaf.hpp>
#include <mithril/leaf/LeafCDB.hpp>
#include <mithril/leaf/LeafSearch.hpp>
#include <mithril/tag/Tag.hpp>
#include <mithril/tag/TagArg.hpp>
#include <mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    bool        search(WebSearcher<Leaf>&ws, folder_k)
    {
        return search(ws, "folder",
            [](std::string_view s) -> Folder
            {
                return arg::folder(s);
            },
            [](Folder f) -> auto
            {
                return cdb::all_leafs(f, Sorted::YES);
            },
            [](Folder f, Leaf l) -> bool
            {
                return cdb::folder(l) != f;
            }
        );
    }
    
    bool        search(WebSearcher<Leaf>&ws, tag_k)
    {
        return search(ws, "tag",
            [](std::string_view s) -> Tag
            {
                return arg::tag(s);
            },
            [](Tag t) -> auto
            {
                return cdb::all_leafs(t, Sorted::YES);
            },
            [](Tag t, Leaf l) -> bool
            {
                return !cdb::tagged(l, t);
            }
        );
    }
    
    LeafVector  search(WebContext&ctx, leaf_k)
    {
        WebSearcher<Leaf>   ws(ctx);
        search(ws, FOLDER);
        search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_leafs(Sorted::YES);
        return ws.data;
    }
}
