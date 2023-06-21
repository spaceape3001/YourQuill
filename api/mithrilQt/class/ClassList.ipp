////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassList.hpp"
#include "ClassModel.hpp"

namespace yq::mithril {
    ClassList::ClassList(all_t, QWidget*parent) : ClassList(ALL, defColumn, parent)
    {
    }
    
    ClassList::ClassList(all_t, Column col, QWidget*parent) : 
        IdListT<Class>(new ClassModel(IdModel::Type::List, ALL), parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Classes");
    }

    ClassList::~ClassList()
    {
    }
    
    ClassModel*          ClassList::model()
    {
        return static_cast<ClassModel*>(m_model);
    }
    
    const ClassModel*    ClassList::model() const
    {
        return static_cast<const ClassModel*>(m_model);
    }
}

