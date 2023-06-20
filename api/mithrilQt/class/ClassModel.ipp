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
    std::span<const Column>  ClassModel::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Name };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    ClassModel::ClassModel(Type t, all_t, std::span<const Column> cols, QObject* parent) : 
        ClassModel(t, Class(), provider::all_classes(), cols, parent)
    {
    }

    ClassModel::ClassModel(Type t, Class rt, IdProvider&& prov, std::span<const Column> cols, QObject*parent) : 
        IdModelT<Class>(t, rt, std::move(prov), parent)
    {
        for(Column c : cols)
            addColumn(c);
        reload();
    }
    
    ClassModel::~ClassModel()
    {
    }
    
    void    ClassModel::addColumn(Column col)
    {
        switch(col){
        case Column::Id:
            addColumn(column::class_id());
            break;
        case Column::Key:
            addColumn(column::class_key());
            break;
        case Column::Name:
            addColumn(column::class_name());
            break;
        default:
            break;
        }
    }
    
}
