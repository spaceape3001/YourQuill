////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/book/BookCDB.hpp>

namespace yq::mithril::cdb {
    std::vector<Book>      all_books(Sorted sorted)
    {
        return blind_remap(all_atoms(book_class(), sorted), Book());
    }
    
    Atom  atom(Book ch)
    {
        return Atom{ ch.id };
    }

    Book  book(Atom at)
    {
        return is(at, book_class()) ? Book(at.id) : Book();
    }

    Class  book_class()
    {
        static const Class  cls = db_class("Book");
        return cls;
    }

    bool  exists(Book ch)
    {
        return is(Atom(ch.id), book_class());
    }

    std::string         key(Book b)
    {
        return key(atom(b));
    }
    
    std::string         title(Book b)
    {
        return name(atom(b));
    }
    
}
