////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/directory/Directory.hpp>
#include <mithril/directory/DirectoryArg.hpp>
#include <mithril/directory/DirectoryCDB.hpp>
#include <mithril/directory/DirectorySearch.hpp>
#include <mithril/folder/FolderArg.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/root/RootArg.hpp>
#include <mithril/root/RootCDB.hpp>
#include <mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    bool            search(WebSearcher<Directory>&ws, folder_k)
    {
        return search(ws, "folder",
            [](std::string_view s) -> Folder
            {
                return arg::folder(s);
            },
            [](Folder f) -> auto
            {
                return cdb::directories(f, Sorted::YES);
            },
            [](Folder f, Directory d) -> bool
            {
                return cdb::folder(d) != f;
            }
        );
    }

    bool            search(WebSearcher<Directory>&ws, parent_k)
    {
        const RootDir*  rt  = arg::root_dir(ws.ctx, "root");
        return search(ws, "parent",
            [rt](std::string_view s) -> Directory
            {
                return arg::directory(s, rt);
            },
            [](Directory p) -> auto
            {
                return cdb::child_directories(p, Sorted::YES);
            },
            [](Directory p, Directory c) -> bool
            {
                return cdb::parent(c) != p;
            }
        );
    }

    bool            search(WebSearcher<Directory>&ws, root_k)
    {
        return search(ws, "root",
            [](std::string_view s) -> const RootDir* 
            {
                return arg::root_dir(s);
            },
            [](const RootDir* rt) -> auto
            {
                return cdb::all_directories(rt, Sorted::YES);
            },
            [](const RootDir* rt, Directory d) -> bool 
            {
                return cdb::root_dir(d) != rt;
            }
        );
    }

    DirectoryVector  search(WebContext&ctx, directory_k)
    {
        WebSearcher<Directory>   ws(ctx);
        search(ws, FOLDER);
        search(ws, ROOT);
        search(ws, PARENT);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_directories(Sorted::YES);
        return ws.data;
    }
}
