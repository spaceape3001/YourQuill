////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>
#include <mithril/book/Book.hpp>

namespace yq::mithril {
    class BookTable : public IdTableT<Book> {
        Q_OBJECT
    public:
        static std::span<const ColumnSpec>  defColumns();

        BookTable(all_t, QWidget*parent=nullptr);
        BookTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent=nullptr);
        BookTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent=nullptr);
        ~BookTable();
    };
}
