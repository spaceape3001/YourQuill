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
    std::optional<IdColumn>     PlaceModel::resolve(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            return column::place_id(opts);
        case Column::Key:
            return column::place_key(opts);
        case Column::Name:
            return column::place_name(opts);
        default:
            return {};
        }
    }

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
        auto    cc  = resolve(col, opts);
        if(cc)
            addColumn(std::move(*cc));
    }
    
    void    PlaceModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }

    void    PlaceModel::setColumn(Column col, ColOpts opts)
    {
        auto    cc = resolve(col, opts);
        if(cc)
            setColumn(std::move(*cc));
    }
}
