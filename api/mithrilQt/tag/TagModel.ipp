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
    std::optional<IdColumn>     TagModel::resolve(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            return column::tag_id(opts);
        case Column::Key:
            return column::tag_key(opts);
        case Column::Name:
            return column::tag_name(opts);
        default:
            return {};
        }
    }

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
        auto    cc  = resolve(col, opts);
        if(cc)
            addColumn(std::move(*cc));
    }

    void    TagModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }
 
    void    TagModel::setColumn(Column col, ColOpts opts)
    {
        auto    cc = resolve(col, opts);
        if(cc)
            setColumn(std::move(*cc));
    }
    
}
