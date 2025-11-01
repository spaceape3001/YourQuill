////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/book/Book.hpp>
#include <mithril/book/BookCDB.hpp>
#include <mithril/book/BookSearch.hpp>
#include <mithril/web/WebSearcher.hpp>

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
