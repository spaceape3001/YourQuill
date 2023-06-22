////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FieldColumn.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/field/FieldCDB.hpp>

namespace yq::mithril::column {
    IdColumn    field_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::field_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    field_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::field_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    field_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::field_name();
        ret.label       = "Name";
        return ret;
    }

    void    reg_field_columns()
    {
        IdColumn::declare<Field>(Column::Id,    field_id);
        IdColumn::declare<Field>(Column::Key,   field_key);
        IdColumn::declare<Field>(Column::Name,  field_name);
    }
    
    YQ_INVOKE(reg_field_columns();)
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
