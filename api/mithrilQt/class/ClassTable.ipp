////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassTable.hpp"
#include "ClassModel.hpp"


namespace yq::mithril {

    ClassTable::ClassTable(all_t, QWidget*parent) : ClassTable(ALL, ClassModel::defColumns(), parent)
    {
    }
    
    ClassTable::ClassTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        ClassTable(ALL, span(columns), parent)
    {
    }
    
    ClassTable::ClassTable(all_t, std::span<const Column> columns, QWidget*parent) : 
        IdTableT<Class>(new ClassModel(IdModel::Type::Table, ALL, columns), parent)
    {
        setWindowTitle("Classes");
    }

    ClassTable::~ClassTable()
    {
    }
    
    
    ClassModel*          ClassTable::model()
    {
        return static_cast<ClassModel*>(m_model);
    }
    
    const ClassModel*    ClassTable::model() const
    {
        return static_cast<const ClassModel*>(m_model);
    }
}

