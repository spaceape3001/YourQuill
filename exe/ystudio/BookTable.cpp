////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/book.hpp>
#include <mithril/atom/AtomCDB.hpp>
#include "BookTable.hpp"


BookTable::BookTable(QWidget*parent) : IdTableT<Book>(
    []() -> std::vector<Book> {
        return cdb::all_books(Sorted::YES);
    }, parent
)
{
    makeColumn<uint64_t> ("ID"sv, [](Book b) -> uint64_t { return b.id; });
    makeColumn<std::string> ("Name", [](Book b) -> std::string {
        return cdb::name(cdb::atom(b));
    });
}

BookTable::~BookTable()
{
}

#include "moc_BookTable.cpp"
