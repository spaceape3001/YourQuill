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
}
