////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PlaceTable.hpp"
#include "PlaceModel.hpp"


namespace yq::mithril {
    std::span<const ColumnSpec>  PlaceTable::defColumns() 
    {
        static ColumnSpec   s_data[] = { { Column::Key, ColOpt::Icon}, Column::Name };
        return std::span<const ColumnSpec>(std::begin(s_data), std::end(s_data));
    }


    PlaceTable::PlaceTable(all_t, QWidget*parent) : PlaceTable(ALL, defColumns(), parent)
    {
    }
    
    PlaceTable::PlaceTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent) : 
        PlaceTable(ALL, span(columns), parent)
    {
    }
    
    PlaceTable::PlaceTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent) : 
        IdTableT<Place>(new PlaceModel(IdModel::Type::Table, ALL), parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Places");
    }

    PlaceTable::~PlaceTable()
    {
    }
    
    
    PlaceModel*          PlaceTable::model()
    {
        return static_cast<PlaceModel*>(m_model);
    }
    
    const PlaceModel*    PlaceTable::model() const
    {
        return static_cast<const PlaceModel*>(m_model);
    }
}

