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
    std::optional<IdColumn>     CategoryModel::resolve(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            return column::category_id(opts);
        case Column::Key:
            return column::category_key(opts);
        case Column::Name:
            return column::category_name(opts);
        default:
            return {};
        }
    }

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
        auto    cc  = resolve(col, opts);
        if(cc)
            addColumn(std::move(*cc));
    }

    void    CategoryModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }

    void    CategoryModel::setColumn(Column col, ColOpts opts)
    {
        auto    cc = resolve(col, opts);
        if(cc)
            setColumn(std::move(*cc));
    }
}
