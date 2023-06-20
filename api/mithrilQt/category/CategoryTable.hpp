////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/column.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class CategoryModel;
    
    class CategoryTable : public IdTableT<Category> {
        Q_OBJECT
    public:
        CategoryTable(all_t, QWidget*parent=nullptr);
        CategoryTable(all_t, std::initializer_list<Column> columns, QWidget*parent=nullptr);
        CategoryTable(all_t, std::span<const Column> columns, QWidget*parent=nullptr);
        ~CategoryTable();
        
        CategoryModel*          model();
        const CategoryModel*    model() const;
    };
}
