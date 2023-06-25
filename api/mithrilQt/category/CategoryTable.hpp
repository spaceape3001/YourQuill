////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>
#include <mithril/category/Category.hpp>

namespace yq::mithril {
    class CategoryTable : public IdTableT<Category> {
        Q_OBJECT
    public:

        static std::span<const ColumnSpec>  defColumns();

        CategoryTable(all_t, QWidget*parent=nullptr);
        CategoryTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent=nullptr);
        CategoryTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent=nullptr);
        ~CategoryTable();
    };
}
