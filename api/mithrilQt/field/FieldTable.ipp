////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FieldTable.hpp"
#include "FieldModel.hpp"


namespace yq::mithril {

    FieldTable::FieldTable(all_t, QWidget*parent) : FieldTable(ALL, FieldModel::defColumns(), parent)
    {
    }
    
    FieldTable::FieldTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        FieldTable(ALL, span(columns), parent)
    {
    }
    
    FieldTable::FieldTable(all_t, std::span<const Column> columns, QWidget*parent) : 
        IdTableT<Field>(new FieldModel(IdModel::Type::Table, ALL, columns), parent)
    {
        setWindowTitle("Fields");
    }

    FieldTable::~FieldTable()
    {
    }
    
    
    FieldModel*          FieldTable::model()
    {
        return static_cast<FieldModel*>(m_model);
    }
    
    const FieldModel*    FieldTable::model() const
    {
        return static_cast<const FieldModel*>(m_model);
    }
}


