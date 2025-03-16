////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/document/Document.hpp>
#include <mithril/document/DocumentArg.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/document/DocumentSearch.hpp>
#include <mithril/folder/FolderArg.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/web/WebSearcher.hpp>

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
