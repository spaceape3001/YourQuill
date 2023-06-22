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
}
