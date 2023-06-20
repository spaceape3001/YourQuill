////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PlaceTable.hpp"
#include "PlaceModel.hpp"


namespace yq::mithril {

    PlaceTable::PlaceTable(all_t, QWidget*parent) : PlaceTable(ALL, PlaceModel::defColumns(), parent)
    {
    }
    
    PlaceTable::PlaceTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        PlaceTable(ALL, span(columns), parent)
    {
    }
    
    PlaceTable::PlaceTable(all_t, std::span<const Column> columns, QWidget*parent) : 
        IdTableT<Place>(new PlaceModel(IdModel::Type::Table, ALL, columns), parent)
    {
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

