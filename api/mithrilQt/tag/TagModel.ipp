////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagModel.hpp"
#include "TagColumn.hpp"
#include <mithril/tag/TagProvider.hpp>

namespace yq::mithril {
    TagModel::TagModel(Type t, all_t, QObject* parent) : 
        TagModel(t, Tag(), provider::all_tags(), parent)
    {
    }

    TagModel::TagModel(Type t, Tag rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Tag>(t, rt, std::move(prov), parent)
    {
    }
    
    TagModel::~TagModel()
    {
    }
    
    void    TagModel::addColumn(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            addColumn(column::tag_id(opts));
            break;
        case Column::Key:
            addColumn(column::tag_key(opts));
            break;
        case Column::Name:
            addColumn(column::tag_name(opts));
            break;
        default:
            break;
        }
    }

    void    TagModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }
    
}
