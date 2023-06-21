////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FieldColumn.hpp"
#include <mithril/field/FieldCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    field_id()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::field_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    field_key()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::field_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    field_name()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::field_name();
        ret.label       = "Name";
        return ret;
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN  field_id()
    {
        return [](Id i) -> QVariant {
            Field    a   = i.as<Field>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  field_key()
    {
        return [](Id i) -> QVariant {
            Field    a   = i.as<Field>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  field_name()
    {
        return [](Id i) -> QVariant {
            Field    a   = i.as<Field>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}
