////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FieldModel.hpp"
#include "FieldColumn.hpp"
#include <mithril/field/FieldProvider.hpp>

namespace yq::mithril {
    FieldModel::FieldModel(Type t, all_t, QObject* parent) : 
        FieldModel(t, Field(), provider::all_fields(), parent)
    {
    }

    FieldModel::FieldModel(Type t, Field rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Field>(t, rt, std::move(prov), parent)
    {
    }
    
    FieldModel::~FieldModel()
    {
    }
    
    void    FieldModel::addColumn(Column col)
    {
        switch(col){
        case Column::Id:
            addColumn(column::field_id());
            break;
        case Column::Key:
            addColumn(column::field_key());
            break;
        case Column::Name:
            addColumn(column::field_name());
            break;
        default:
            break;
        }
    }
    
    void    FieldModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }
}