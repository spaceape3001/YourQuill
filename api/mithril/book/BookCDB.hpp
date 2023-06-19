////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/book/Book.hpp>

namespace yq::mithril {        
    namespace cdb {
        std::vector<Book>   all_books(Sorted sorted=Sorted());
        Atom                atom(Book);
        Book                book(Atom);
        Class               book_class();
        std::string         key(Book);
        std::string         title(Book);
        bool                exists(Book);
        bool                exists_book(uint64_t);
    }
}
