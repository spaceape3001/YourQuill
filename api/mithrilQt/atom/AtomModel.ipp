////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AtomModel.hpp"
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
}
