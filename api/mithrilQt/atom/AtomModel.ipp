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
    std::optional<IdColumn>     AtomModel::resolve(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            return column::atom_id(opts);
        case Column::Key:
            return column::atom_key(opts);
        case Column::Name:
            return column::atom_name(opts);
        default:
            return {};
        }
    }

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
        auto    cc  = resolve(col, opts);
        if(cc)
            addColumn(std::move(*cc));
    }

    void    AtomModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }

    void    AtomModel::setColumn(Column col, ColOpts opts)
    {
        auto    cc = resolve(col, opts);
        if(cc)
            setColumn(std::move(*cc));
    }
}
