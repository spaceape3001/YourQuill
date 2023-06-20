////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootTable.hpp"
#include "RootModel.hpp"


namespace yq::mithril {

    RootTable::RootTable(all_t, QWidget*parent) : RootTable(ALL, RootModel::defColumns(), parent)
    {
    }
    
    RootTable::RootTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        RootTable(ALL, span(columns), parent)
    {
    }
    
    RootTable::RootTable(all_t, std::span<const Column> columns, QWidget*parent) : 
        IdTableT<Root>(new RootModel(IdModel::Type::Table, ALL, columns), parent)
    {
        setWindowTitle("Roots");
    }

    RootTable::~RootTable()
    {
    }
    
    
    RootModel*          RootTable::model()
    {
        return static_cast<RootModel*>(m_model);
    }
    
    const RootModel*    RootTable::model() const
    {
        return static_cast<const RootModel*>(m_model);
    }
}

