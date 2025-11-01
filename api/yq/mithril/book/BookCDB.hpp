////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/preamble.hpp>
#include <yq/mithril/book/Book.hpp>

namespace yq::mithril::cdb {
    std::vector<Book>   all_books(Sorted sorted=Sorted());
    Atom                atom(Book);
    Book                book(Atom);
    Book                book(std::string_view);
    Book                book(uint64_t);
    Class               book_class();
    std::string         brief(Book);
    Image               icon(Book);
    std::string         key(Book);
    std::string         name(Book);
    std::string         title(Book);
    bool                exists(Book);
    bool                exists_book(uint64_t);
}
