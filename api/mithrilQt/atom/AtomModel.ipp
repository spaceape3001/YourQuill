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
    std::span<const Column>  AtomModel::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Name };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    AtomModel::AtomModel(Type t, all_t, std::span<const Column> cols, QObject* parent) : 
        AtomModel(t, Atom(), provider::all_atoms(), cols, parent)
    {
    }

    AtomModel::AtomModel(Type t, Atom rt, IdProvider&& prov, std::span<const Column> cols, QObject*parent) : 
        IdModelT<Atom>(t, rt, std::move(prov), parent)
    {
        for(Column c : cols)
            addColumn(c);
        reload();
    }
    
    AtomModel::~AtomModel()
    {
    }
    
    void    AtomModel::addColumn(Column col)
    {
        switch(col){
        case Column::Id:
            addColumn(column::atom_id());
            break;
        case Column::Key:
            addColumn(column::atom_key());
            break;
        case Column::Name:
            addColumn(column::atom_name());
            break;
        default:
            break;
        }
    }
    
}
