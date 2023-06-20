////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PlaceModel.hpp"
#include "PlaceColumn.hpp"
#include <mithril/place/PlaceProvider.hpp>

namespace yq::mithril {
    std::span<const Column>  PlaceModel::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Name };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    PlaceModel::PlaceModel(Type t, all_t, std::span<const Column> cols, QObject* parent) : 
        PlaceModel(t, Place(), provider::all_places(), cols, parent)
    {
    }

    PlaceModel::PlaceModel(Type t, Place rt, IdProvider&& prov, std::span<const Column> cols, QObject*parent) : 
        IdModelT<Place>(t, rt, std::move(prov), parent)
    {
        for(Column c : cols)
            addColumn(c);
        reload();
    }
    
    PlaceModel::~PlaceModel()
    {
    }
    
    void    PlaceModel::addColumn(Column col)
    {
        switch(col){
        case Column::Id:
            addColumn(column::place_id());
            break;
        case Column::Key:
            addColumn(column::place_key());
            break;
        case Column::Name:
            addColumn(column::place_name());
            break;
        default:
            break;
        }
    }
    
}
