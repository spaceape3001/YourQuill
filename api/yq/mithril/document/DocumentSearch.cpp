////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/document/Document.hpp>
#include <yq/mithril/document/DocumentArg.hpp>
#include <yq/mithril/document/DocumentCDB.hpp>
#include <yq/mithril/document/DocumentSearch.hpp>
#include <yq/mithril/folder/FolderArg.hpp>
#include <yq/mithril/folder/FolderCDB.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    bool            search(WebSearcher<Document>&ws, folder_k)
    {
        return search(ws, "folder",
            [](std::string_view s) -> Folder
            {
                return arg::folder(s);
            },
            [](Folder f) -> auto
            {
                return cdb::child_documents(f, Sorted::YES);
            },
            [](Folder f, Document d) -> bool
            {
                return cdb::folder(d) != f;
            }
        );
    }

    DocumentVector  search(WebContext&ctx, document_k)
    {
        WebSearcher<Document>   ws(ctx);
        search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_documents(Sorted::YES);
        return ws.data;
    }
}
