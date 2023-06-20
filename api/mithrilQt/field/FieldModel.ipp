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
    std::span<const Column>  FieldModel::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Name };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    FieldModel::FieldModel(Type t, all_t, std::span<const Column> cols, QObject* parent) : 
        FieldModel(t, Field(), provider::all_fields(), cols, parent)
    {
    }

    FieldModel::FieldModel(Type t, Field rt, IdProvider&& prov, std::span<const Column> cols, QObject*parent) : 
        IdModelT<Field>(t, rt, std::move(prov), parent)
    {
        for(Column c : cols)
            addColumn(c);
        reload();
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
    
}
