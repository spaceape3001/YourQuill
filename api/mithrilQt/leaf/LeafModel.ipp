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
    std::span<const Column>  LeafModel::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Title };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    LeafModel::LeafModel(Type t, all_t, std::span<const Column> cols, QObject* parent) : 
        LeafModel(t, Leaf(), provider::all_leafs(), cols, parent)
    {
    }

    LeafModel::LeafModel(Type t, Leaf rt, IdProvider&& prov, std::span<const Column> cols, QObject*parent) : 
        IdModelT<Leaf>(t, rt, std::move(prov), parent)
    {
        for(Column c : cols)
            addColumn(c);
        reload();
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
    
}
