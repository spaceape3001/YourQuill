////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootModel.hpp"
#include "RootColumn.hpp"
#include <mithril/root/RootProvider.hpp>

namespace yq::mithril {
    RootModel::RootModel(Type t, all_t, QObject* parent) : 
        RootModel(t, Root(), provider::all_roots(), parent)
    {
    }

    RootModel::RootModel(Type t, Root rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Root>(t, rt, std::move(prov), parent)
    {
    }
    
    RootModel::~RootModel()
    {
    }
    
    void    RootModel::addColumn(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            addColumn(column::root_id(opts));
            break;
        case Column::Key:
            addColumn(column::root_key(opts));
            break;
        case Column::Name:
            addColumn(column::root_name(opts));
            break;
        case Column::Path:
            addColumn(column::root_path(opts));
            break;
        case Column::Template:
            addColumn(column::root_template(opts));
            break;
        default:
            break;
        }
    }
    
    void    RootModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }
}
