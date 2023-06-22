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
    //std::optional<IdColumn>     ClassModel::resolve(Column col, ColOpts opts)
    //{
        //switch(col){
        //case Column::Id:
            //return column::class_id(opts);
        //case Column::Key:
            //return column::class_key(opts);
        //case Column::Name:
            //return column::class_name(opts);
        //default:
            //return {};
        //}
    //}

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
    
    //void    ClassModel::addColumn(Column col, ColOpts opts)
    //{
        //auto    cc  = resolve(col, opts);
        //if(cc)
            //addColumn(std::move(*cc));
    //}

    //void    ClassModel::addColumns(std::span<const Column> columns)
    //{
        //for(Column c : columns)
            //addColumn(c);
    //}

    //void    ClassModel::setColumn(Column col, ColOpts opts)
    //{
        //auto    cc = resolve(col, opts);
        //if(cc)
            //setColumn(std::move(*cc));
    //}
    
}
