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
    CategoryModel::CategoryModel(Type t, all_t, QObject* parent) : 
        CategoryModel(t, Category(), provider::all_categories(), parent)
    {
    }

    CategoryModel::CategoryModel(Type t, Category rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Category>(t, rt, std::move(prov), parent)
    {
    }
    
    CategoryModel::~CategoryModel()
    {
    }
    
    void    CategoryModel::addColumn(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            addColumn(column::category_id(opts));
            break;
        case Column::Key:
            addColumn(column::category_key(opts));
            break;
        case Column::Name:
            addColumn(column::category_name(opts));
            break;
        default:
            break;
        }
    }

    void    CategoryModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }
}
