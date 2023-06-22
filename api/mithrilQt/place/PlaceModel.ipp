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
}
