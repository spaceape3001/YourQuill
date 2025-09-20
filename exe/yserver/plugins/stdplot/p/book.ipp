////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/book/BookJson.hpp>
#include <mithril/book/BookSearch.hpp>
#include <yq/typedef/string.hpp>
#include <mithril/wksp/Workspace.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_book(WebContext& ctx)
        {
            Book    v   = arg::book(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_book_key(WebContext& ctx)
        {
            Book    v   = arg::book(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_books(WebContext& ctx)
        {
            BookVector  ret = search(ctx, BOOK);
            return json{
                { "books", json_(ret) }
            };
        }

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void    p_dev_books(WebHtml& h)
        {
            h.title() << "All Books";
            dev_table(h, all_books(Sorted::YES));
        }
        

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_book_pages()
        {
            reg_webpage<p_api_book>("/api/book"sv).argument("ID", "Book ID");
            reg_webpage<p_api_book_key>("/api/book/key"sv).argument("ID", "Book ID");
            reg_webpage<p_api_books>("/api/books"sv);
            
            reg_webpage<p_dev_books>("/dev/books");

            reg_webtemplate("/books", wksp::shared("www/books.ht"sv));
        }
        
}
