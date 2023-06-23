////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LeafTable.hpp"
#include "LeafModel.hpp"


namespace yq::mithril {
    std::span<const ColumnSpec>  LeafTable::defColumns() 
    {
        static ColumnSpec   s_data[] = { { Column::Key, ColOpt::Icon}, Column::Title };
        return std::span<const ColumnSpec>(std::begin(s_data), std::end(s_data));
    }


    LeafTable::LeafTable(all_t, QWidget*parent) : LeafTable(ALL, defColumns(), parent)
    {
    }
    
    LeafTable::LeafTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent) : 
        LeafTable(ALL, span(columns), parent)
    {
    }
    
    LeafTable::LeafTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent) : 
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

