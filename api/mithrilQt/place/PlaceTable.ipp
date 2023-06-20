////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PlaceTable.hpp"
#include "PlaceModel.hpp"


namespace yq::mithril {
    std::span<const Column>  PlaceTable::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Name };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }


    PlaceTable::PlaceTable(all_t, QWidget*parent) : PlaceTable(ALL, defColumns(), parent)
    {
    }
    
    PlaceTable::PlaceTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        PlaceTable(ALL, span(columns), parent)
    {
    }
    
    PlaceTable::PlaceTable(all_t, std::span<const Column> columns, QWidget*parent) : 
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

