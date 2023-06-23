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
#include <mithril/image/Image.hpp>
#include <mithrilQt/image/ImageUtils.hpp>
#include <QIcon>

namespace yq::mithril::column {
    IdColumn    field_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::field_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::field_icon();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    field_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::field_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::field_icon();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    field_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::field_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::field_icon();
        ret.label               = "Name";
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

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  field_icon()
    {
        static QIcon    qico(":/generic/field.svg");
        return [](Id i) -> QVariant {
            Field   a   = i.as<Field>();
            if(!a)
                return QVariant();
            Image   img = cdb::icon(a);
            if(img)
                return qIcon(img);
            return qico;
        };
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
