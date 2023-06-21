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
    
    void    LeafModel::addColumn(Column col)
    {
        switch(col){
        case Column::Id:
            addColumn(column::leaf_id());
            break;
        case Column::Key:
            addColumn(column::leaf_key());
            break;
        case Column::Title:
            addColumn(column::leaf_title());
            break;
        default:
            break;
        }
    }
    
    void    LeafModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }
}