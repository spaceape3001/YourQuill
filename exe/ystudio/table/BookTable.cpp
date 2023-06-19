////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/book.hpp>
#include "BookTable.hpp"


BookTable::BookTable(QWidget*parent) : IdTableT<Book>(
    []() -> std::vector<Book> {
        return cdb::all_books(Sorted::YES);
    }, parent
)
{
    makeColumn<std::string> ("Key", [](Book b) -> std::string {
        return cdb::key(b);
    });
    makeColumn<std::string> ("Title", [](Book b) -> std::string {
        return cdb::title(b);
    });

    setWindowTitle(tr("Books"));
}

BookTable::~BookTable()
{
}

#include "moc_BookTable.cpp"
