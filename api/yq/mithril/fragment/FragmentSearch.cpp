////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/directory/Directory.hpp>
#include <yq/mithril/directory/DirectoryArg.hpp>
#include <yq/mithril/directory/DirectoryCDB.hpp>

#include <yq/mithril/document/Document.hpp>
#include <yq/mithril/document/DocumentArg.hpp>
#include <yq/mithril/document/DocumentCDB.hpp>

#include <yq/mithril/folder/Folder.hpp>
#include <yq/mithril/folder/FolderArg.hpp>
#include <yq/mithril/folder/FolderCDB.hpp>

#include <yq/mithril/fragment/Fragment.hpp>
#include <yq/mithril/fragment/FragmentCDB.hpp>
#include <yq/mithril/fragment/FragmentSearch.hpp>

#include <yq/mithril/root/RootArg.hpp>
#include <yq/mithril/root/RootCDB.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    bool            search(WebSearcher<Fragment>&ws, directory_k)
    {
        const RootDir*  rt  = arg::root_dir(ws.ctx, "root");
        return search(ws, "directory",
            [rt](std::string_view s) -> Directory
            {
                return arg::directory(s, rt);
            },
            [](Directory p) -> auto
            {
                return cdb::child_fragments(p, Sorted(Sorted::YES));
            },
            [](Directory p, Fragment c) -> bool
            {
                return cdb::parent(c) != p;
            }
        );
    }
    
    bool            search(WebSearcher<Fragment>&ws, document_k)
    {
        return search(ws, "document",
            [](std::string_view s) -> Document
            {
                return arg::document(s);
            },
            [](Document d) -> auto
            {
                return cdb::fragments(d, Sorted(Sorted::YES));
            },
            [](Document d, Fragment c) -> bool
            {
                return cdb::document(c) != d;
            }
        );
    }

    bool            search(WebSearcher<Fragment>&ws, folder_k)
    {
        return search(ws, "folder",
            [](std::string_view s) -> Folder
            {
                return arg::folder(s);
            },
            [](Folder f) -> auto
            {
                return cdb::child_fragments(f, Sorted::YES);
            },
            [](Folder p, Fragment c) -> bool
            {
                return cdb::folder(c) != p;
            }
        );
    }

    bool            search(WebSearcher<Fragment>&ws, parent_k)
    {
        const RootDir*  rt  = arg::root_dir(ws.ctx, "root");
        return search(ws, "parent",
            [rt](std::string_view s) -> Directory
            {
                return arg::directory(s, rt);
            },
            [](Directory p) -> auto
            {
                return cdb::child_fragments(p, Sorted::YES);
            },
            [](Directory p, Fragment c) -> bool
            {
                return cdb::parent(c) != p;
            }
        );
    }

    bool            search(WebSearcher<Fragment>&ws, root_k)
    {
        return search(ws, "root",
            [](std::string_view s) -> const RootDir* 
            {
                return arg::root_dir(s);
            },
            [](const RootDir* rt) -> auto
            {
                return cdb::all_fragments(rt, Sorted::YES);
            },
            [](const RootDir* rt, Fragment f) -> bool 
            {
                return cdb::root_dir(f) != rt;
            }
        );
    }

    FragmentVector  search(WebContext&ctx, fragment_k)
    {
        WebSearcher<Fragment>   ws(ctx);
        search(ws, DIRECTORY);
        search(ws, DOCUMENT);
        search(ws, FOLDER);
        search(ws, PARENT);
        search(ws, ROOT);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_fragments(Sorted::YES);
        return ws.data;
    }
}
