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
    std::span<const Column>  TagModel::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Name };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    TagModel::TagModel(Type t, all_t, std::span<const Column> cols, QObject* parent) : 
        TagModel(t, Tag(), provider::all_tags(), cols, parent)
    {
    }

    TagModel::TagModel(Type t, Tag rt, IdProvider&& prov, std::span<const Column> cols, QObject*parent) : 
        IdModelT<Tag>(t, rt, std::move(prov), parent)
    {
        for(Column c : cols)
            addColumn(c);
        reload();
    }
    
    TagModel::~TagModel()
    {
    }
    
    void    TagModel::addColumn(Column col)
    {
        switch(col){
        case Column::Id:
            addColumn(column::tag_id());
            break;
        case Column::Key:
            addColumn(column::tag_key());
            break;
        case Column::Name:
            addColumn(column::tag_name());
            break;
        default:
            break;
        }
    }
    
}
