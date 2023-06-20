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
    
        static std::span<const Column>  defColumns();
    
        CategoryModel(Type t) : CategoryModel(t, ALL) {}
        CategoryModel(Type t, all_t, std::span<const Column> cols=defColumns(), QObject* parent=nullptr);
        ~CategoryModel();
        
        using IdModelT<Category>::addColumn;
        void    addColumn(Column);
        
    private:
        CategoryModel(Type t, Category, IdProvider&&, std::span<const Column> cols, QObject*parent);
    };
}
