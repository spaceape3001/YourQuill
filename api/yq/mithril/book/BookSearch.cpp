////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/book/Book.hpp>
#include <yq/mithril/book/BookCDB.hpp>
#include <yq/mithril/book/BookSearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    BookVector  search(WebContext&ctx, book_k)
    {
        WebSearcher<Book>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_books(Sorted::YES);
        return ws.data;
    }
}
