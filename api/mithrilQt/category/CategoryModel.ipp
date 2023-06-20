////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CategoryModel.hpp"
#include "CategoryColumn.hpp"
#include <mithril/category/CategoryProvider.hpp>

namespace yq::mithril {
    std::span<const Column>  CategoryModel::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Name };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    CategoryModel::CategoryModel(Type t, all_t, std::span<const Column> cols, QObject* parent) : 
        CategoryModel(t, Category(), provider::all_categories(), cols, parent)
    {
    }

    CategoryModel::CategoryModel(Type t, Category rt, IdProvider&& prov, std::span<const Column> cols, QObject*parent) : 
        IdModelT<Category>(t, rt, std::move(prov), parent)
    {
        for(Column c : cols)
            addColumn(c);
        reload();
    }
    
    CategoryModel::~CategoryModel()
    {
    }
    
    void    CategoryModel::addColumn(Column col)
    {
        switch(col){
        case Column::Id:
            addColumn(column::category_id());
            break;
        case Column::Key:
            addColumn(column::category_key());
            break;
        case Column::Name:
            addColumn(column::category_name());
            break;
        default:
            break;
        }
    }
    
}
