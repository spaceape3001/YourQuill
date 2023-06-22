////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LeafModel.hpp"
#include "LeafColumn.hpp"
#include <mithril/leaf/LeafProvider.hpp>

namespace yq::mithril {
    LeafModel::LeafModel(Type t, all_t, QObject* parent) : 
        LeafModel(t, Leaf(), provider::all_leafs(), parent)
    {
    }

    LeafModel::LeafModel(Type t, Leaf rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Leaf>(t, rt, std::move(prov), parent)
    {
    }
    
    LeafModel::~LeafModel()
    {
    }
}
