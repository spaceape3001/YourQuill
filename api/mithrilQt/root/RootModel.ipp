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
    std::span<const Column>  RootModel::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Path };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    RootModel::RootModel(Type t, all_t, std::span<const Column> cols, QObject* parent) : 
        RootModel(t, Root(), provider::all_roots(), cols, parent)
    {
    }

    RootModel::RootModel(Type t, Root rt, IdProvider&& prov, std::span<const Column> cols, QObject*parent) : 
        IdModelT<Root>(t, rt, std::move(prov), parent)
    {
        for(Column c : cols)
            addColumn(c);
        reload();
    }
    
    RootModel::~RootModel()
    {
    }
    
    void    RootModel::addColumn(Column col)
    {
        switch(col){
        case Column::Id:
            addColumn(column::root_id());
            break;
        case Column::Key:
            addColumn(column::root_key());
            break;
        case Column::Name:
            addColumn(column::root_name());
            break;
        case Column::Path:
            addColumn(column::root_path());
            break;
        case Column::Template:
            addColumn(column::root_template());
            break;
        default:
            break;
        }
    }
    
}
