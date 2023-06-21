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
    PlaceModel::PlaceModel(Type t, all_t, QObject* parent) : 
        PlaceModel(t, Place(), provider::all_places(), parent)
    {
    }

    PlaceModel::PlaceModel(Type t, Place rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Place>(t, rt, std::move(prov), parent)
    {
    }
    
    PlaceModel::~PlaceModel()
    {
    }
    
    void    PlaceModel::addColumn(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            addColumn(column::place_id(opts));
            break;
        case Column::Key:
            addColumn(column::place_key(opts));
            break;
        case Column::Name:
            addColumn(column::place_name(opts));
            break;
        default:
            break;
        }
    }
    
    void    PlaceModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }
}
