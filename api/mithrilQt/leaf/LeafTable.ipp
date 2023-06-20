////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LeafTable.hpp"
#include "LeafModel.hpp"


namespace yq::mithril {

    LeafTable::LeafTable(all_t, QWidget*parent) : LeafTable(ALL, LeafModel::defColumns(), parent)
    {
    }
    
    LeafTable::LeafTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        LeafTable(ALL, span(columns), parent)
    {
    }
    
    LeafTable::LeafTable(all_t, std::span<const Column> columns, QWidget*parent) : 
        IdTableT<Leaf>(new LeafModel(IdModel::Type::Table, ALL, columns), parent)
    {
        setWindowTitle("Leafs");
    }

    LeafTable::~LeafTable()
    {
    }
    
    
    LeafModel*          LeafTable::model()
    {
        return static_cast<LeafModel*>(m_model);
    }
    
    const LeafModel*    LeafTable::model() const
    {
        return static_cast<const LeafModel*>(m_model);
    }
}

