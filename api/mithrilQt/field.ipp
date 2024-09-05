////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "field.hpp"
#include <yq-toolbox/basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/field/FieldCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Field fld)
    {
        static QIcon    qico(":/generic/field.svg");
        if(!fld)
            return QIcon();
        Image   img = cdb::icon(fld);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    field_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::field_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::field_icon();
        ret.fnCompare           = compareFN::field_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    field_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::field_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::field_icon();
        ret.fnCompare           = compareFN::field_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    field_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::field_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::field_icon();
        ret.fnCompare           = compareFN::field_name();
        ret.label               = "Name";
        return ret;
    }

    void    reg_field_columns()
    {
        IdColumn::declare<Field>(Column::Id,    field_id);
        IdColumn::declare<Field>(Column::Key,   field_key);
        IdColumn::declare<Field>(Column::Name,  field_name);

        IdColumn::set_defaultList<Field>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Field>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_field_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN field_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Field>().id, b.as<Field>().id);
        };
    }
    
    IdColumn::CompareFN field_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Field>()), cdb::key(b.as<Field>()));
        };
    }
    
    IdColumn::CompareFN field_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Field>()), cdb::name(b.as<Field>()));
        };
    }
}



namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  field_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Field>());
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
