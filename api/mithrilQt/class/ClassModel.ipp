////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassModel.hpp"
#include "ClassColumn.hpp"
#include <mithril/class/ClassProvider.hpp>

namespace yq::mithril {
    ClassModel::ClassModel(Type t, all_t, QObject* parent) : 
        ClassModel(t, Class(), provider::all_classes(), parent)
    {
    }

    ClassModel::ClassModel(Type t, Class rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Class>(t, rt, std::move(prov), parent)
    {
    }
    
    ClassModel::~ClassModel()
    {
    }
    
    void    ClassModel::addColumn(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            addColumn(column::class_id(opts));
            break;
        case Column::Key:
            addColumn(column::class_key(opts));
            break;
        case Column::Name:
            addColumn(column::class_name(opts));
            break;
        default:
            break;
        }
    }

    void    ClassModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }
    
}
