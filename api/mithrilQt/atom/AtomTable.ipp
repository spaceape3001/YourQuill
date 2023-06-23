////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AtomTable.hpp"
#include "AtomModel.hpp"


namespace yq::mithril {

    std::span<const ColumnSpec>  AtomTable::defColumns() 
    {
        static ColumnSpec   s_data[] = { {Column::Key, ColOpt::Icon}, Column::Name };
        return std::span<const ColumnSpec>(std::begin(s_data), std::end(s_data));
    }

    AtomTable::AtomTable(all_t, QWidget*parent) : AtomTable(ALL, defColumns(), parent)
    {
    }
    
    AtomTable::AtomTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent) : 
        AtomTable(ALL, span(columns), parent)
    {
    }
    
    AtomTable::AtomTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent) : 
        IdTableT<Atom>(new AtomModel(IdModel::Type::Table, ALL), parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Atoms");
    }

    AtomTable::~AtomTable()
    {
    }
    
    
    AtomModel*          AtomTable::model()
    {
        return static_cast<AtomModel*>(m_model);
    }
    
    const AtomModel*    AtomTable::model() const
    {
        return static_cast<const AtomModel*>(m_model);
    }
}

