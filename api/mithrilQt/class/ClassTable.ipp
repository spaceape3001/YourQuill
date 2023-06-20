////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassTable.hpp"
#include "ClassModel.hpp"


namespace yq::mithril {
    std::span<const Column>  ClassTable::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Name };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }


    ClassTable::ClassTable(all_t, QWidget*parent) : ClassTable(ALL, defColumns(), parent)
    {
    }
    
    ClassTable::ClassTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        ClassTable(ALL, span(columns), parent)
    {
    }
    
    ClassTable::ClassTable(all_t, std::span<const Column> columns, QWidget*parent) : 
        IdTableT<Class>(new ClassModel(IdModel::Type::Table, ALL), parent)
    {
        model()->addColumns(columns);
        model()->reload();
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

