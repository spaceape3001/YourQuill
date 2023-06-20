////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AtomTable.hpp"
#include "AtomModel.hpp"


namespace yq::mithril {

    AtomTable::AtomTable(all_t, QWidget*parent) : AtomTable(ALL, AtomModel::defColumns(), parent)
    {
    }
    
    AtomTable::AtomTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        AtomTable(ALL, span(columns), parent)
    {
    }
    
    AtomTable::AtomTable(all_t, std::span<const Column> columns, QWidget*parent) : 
        IdTableT<Atom>(new AtomModel(IdModel::Type::Table, ALL, columns), parent)
    {
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

