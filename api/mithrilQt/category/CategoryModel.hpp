////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/category/Category.hpp>
#include <mithrilQt/column.hpp>

namespace yq::mithril {
    class CategoryModel : public IdModelT<Category> {
        Q_OBJECT
    public:
    
        CategoryModel(Type t, QObject* parent=nullptr) : CategoryModel(t, ALL, parent) {}
        CategoryModel(Type t, all_t, QObject* parent=nullptr);
        ~CategoryModel();
        
        using IdModelT<Category>::addColumn;
        void    addColumn(Column);
        void    addColumns(std::span<const Column> columns);
        
    private:
        CategoryModel(Type t, Category, IdProvider&&, QObject*parent);
    };
}
