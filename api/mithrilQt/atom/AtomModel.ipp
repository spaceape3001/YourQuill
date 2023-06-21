////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AtomModel.hpp"
#include "AtomColumn.hpp"
#include <mithril/atom/AtomProvider.hpp>

namespace yq::mithril {
    AtomModel::AtomModel(Type t, all_t, QObject* parent) : 
        AtomModel(t, Atom(), provider::all_atoms(), parent)
    {
    }

    AtomModel::AtomModel(Type t, Atom rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Atom>(t, rt, std::move(prov), parent)
    {
    }
    
    AtomModel::~AtomModel()
    {
    }
    
    void    AtomModel::addColumn(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            addColumn(column::atom_id(opts));
            break;
        case Column::Key:
            addColumn(column::atom_key(opts));
            break;
        case Column::Name:
            addColumn(column::atom_name(opts));
            break;
        default:
            break;
        }
    }

    void    AtomModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }
}
