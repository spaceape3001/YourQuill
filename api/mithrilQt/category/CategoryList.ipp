////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CategoryList.hpp"
#include "CategoryModel.hpp"

namespace yq::mithril {
    CategoryList::CategoryList(all_t, QWidget*parent) : CategoryList(ALL, defColumn, parent)
    {
    }
    
    CategoryList::CategoryList(all_t, Column col, QWidget*parent) : 
        IdListT<Category>(new CategoryModel(IdModel::Type::List, ALL), parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Categories");
    }

    CategoryList::~CategoryList()
    {
    }
    
    CategoryModel*          CategoryList::model()
    {
        return static_cast<CategoryModel*>(m_model);
    }
    
    const CategoryModel*    CategoryList::model() const
    {
        return static_cast<const CategoryModel*>(m_model);
    }
}

