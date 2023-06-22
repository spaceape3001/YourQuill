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
}
