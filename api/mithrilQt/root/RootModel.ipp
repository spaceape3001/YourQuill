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
    std::optional<IdColumn>     RootModel::resolve(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            return column::root_id(opts);
        case Column::Key:
            return column::root_key(opts);
        case Column::Name:
            return column::root_name(opts);
        case Column::Path:
            return column::root_path(opts);
        case Column::Template:
            return column::root_template(opts);
        default:
            return {};
        }
    }

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
        auto    cc  = resolve(col, opts);
        if(cc)
            addColumn(std::move(*cc));
    }
    
    void    RootModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }

    void    RootModel::setColumn(Column col, ColOpts opts)
    {
        auto    cc = resolve(col, opts);
        if(cc)
            setColumn(std::move(*cc));
    }
}
