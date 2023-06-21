////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FragmentModel.hpp"
#include "FragmentColumn.hpp"
#include <mithril/fragment/FragmentProvider.hpp>

namespace yq::mithril {
    FragmentModel::FragmentModel(Type t, all_t, QObject* parent) : 
        FragmentModel(t, Fragment(), provider::all_fragments(), parent)
    {
    }

    FragmentModel::FragmentModel(Type t, Fragment rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Fragment>(t, rt, std::move(prov), parent)
    {
    }
    
    FragmentModel::~FragmentModel()
    {
    }
    
    void    FragmentModel::addColumn(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            addColumn(column::fragment_id(opts));
            break;
        case Column::Key:
            addColumn(column::fragment_key(opts));
            break;
        case Column::Name:
            addColumn(column::fragment_name(opts));
            break;
        case Column::Path:
            addColumn(column::fragment_path(opts));
            break;
        default:
            break;
        }
    }

    void    FragmentModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }
    
}
