////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FieldList.hpp"
#include "FieldModel.hpp"

namespace yq::mithril {
    FieldList::FieldList(all_t, QWidget*parent) : FieldList(ALL, defColumn, parent)
    {
    }
    
    FieldList::FieldList(all_t, Column col, QWidget*parent) : 
        IdListT<Field>(new FieldModel(IdModel::Type::List, ALL), parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Fields");
    }

    FieldList::~FieldList()
    {
    }
    
    FieldModel*          FieldList::model()
    {
        return static_cast<FieldModel*>(m_model);
    }
    
    const FieldModel*    FieldList::model() const
    {
        return static_cast<const FieldModel*>(m_model);
    }
}

