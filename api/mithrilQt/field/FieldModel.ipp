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
}
