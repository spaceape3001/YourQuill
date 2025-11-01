////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/atom/AtomCDB.hpp>
#include <yq/mithril/class/ClassCDB.hpp>
#include <yq/mithril/book/BookCDB.hpp>

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

    Book  book(std::string_view sv)
    {
        return book(atom(sv));
    }

    Book  book(uint64_t i)
    {
        return exists_book(i) ? Book(i) : Book{};
    }

    Class  book_class()
    {
        static const Class  cls = db_class("Book");
        return cls;
    }

    std::string brief(Book b)
    {
        return brief(atom(b));
    }

    bool  exists(Book ch)
    {
        return is(Atom(ch.id), book_class());
    }

    bool  exists_book(uint64_t i)
    {
        return is(atom(i), book_class());
    }

    Image               icon(Book b)
    {
        return icon(atom(b));
    }
    
    std::string         key(Book b)
    {
        return key(atom(b));
    }
    
    std::string         name(Book b)
    {
        return name(atom(b));
    }

    std::string         title(Book b)
    {
        return name(atom(b));
    }
    
}
