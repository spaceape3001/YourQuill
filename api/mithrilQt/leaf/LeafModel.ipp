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
    std::optional<IdColumn>     LeafModel::resolve(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            return column::leaf_id(opts);
        case Column::Key:
            return column::leaf_key(opts);
        case Column::Title:
            column::leaf_title(opts);
        default:
            return {};
        }
    }

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
    
    void    LeafModel::addColumn(Column col, ColOpts opts)
    {
        auto    cc  = resolve(col, opts);
        if(cc)
            addColumn(std::move(*cc));
    }
    
    void    LeafModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }

    void    LeafModel::setColumn(Column col, ColOpts opts)
    {
        auto    cc = resolve(col, opts);
        if(cc)
            setColumn(std::move(*cc));
    }
}
