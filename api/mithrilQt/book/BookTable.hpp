////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class BookModel;
    
    class BookTable : public IdTableT<Book> {
        Q_OBJECT
    public:
        static std::span<const Column>  defColumns();

        BookTable(all_t, QWidget*parent=nullptr);
        BookTable(all_t, std::initializer_list<Column> columns, QWidget*parent=nullptr);
        BookTable(all_t, std::span<const Column> columns, QWidget*parent=nullptr);
        ~BookTable();
        
        BookModel*          model();
        const BookModel*    model() const;
    };
}