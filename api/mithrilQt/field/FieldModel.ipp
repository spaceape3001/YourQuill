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
    std::optional<IdColumn>     FieldModel::resolve(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            return column::field_id(opts);
        case Column::Key:
            return column::field_key(opts);
        case Column::Name:
            return column::field_name(opts);
        default:
            return {};
        }
    }

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
    
    void    FieldModel::addColumn(Column col, ColOpts opts)
    {
        auto    cc  = resolve(col, opts);
        if(cc)
            addColumn(std::move(*cc));
    }
    
    void    FieldModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }

    void    FieldModel::setColumn(Column col, ColOpts opts)
    {
        auto    cc = resolve(col, opts);
        if(cc)
            setColumn(std::move(*cc));
    }
}
