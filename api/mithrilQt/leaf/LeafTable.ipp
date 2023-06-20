////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LeafTable.hpp"
#include "LeafModel.hpp"


namespace yq::mithril {
    std::span<const Column>  LeafTable::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Title };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }


    LeafTable::LeafTable(all_t, QWidget*parent) : LeafTable(ALL, defColumns(), parent)
    {
    }
    
    LeafTable::LeafTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        LeafTable(ALL, span(columns), parent)
    {
    }
    
    LeafTable::LeafTable(all_t, std::span<const Column> columns, QWidget*parent) : 
        IdTableT<Leaf>(new LeafModel(IdModel::Type::Table, ALL), parent)
    {
        model()->addColumns(columns);
        model()->reload();
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

