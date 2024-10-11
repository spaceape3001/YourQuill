////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "thing.hpp"
#include <yq/core/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/thing/ThingCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Thing p)
    {
        static QIcon    qico(":/generic/thing.svg");
        if(!p)
            return QIcon();
        Image   img = cdb::icon(p);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    thing_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::thing_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::thing_icon();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    thing_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::thing_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::thing_icon();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    thing_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::thing_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::thing_icon();
        ret.label               = "Name";
        return ret;
    }

    void    reg_thing_columns()
    {
        IdColumn::declare<Thing>(Column::Id,    thing_id);
        IdColumn::declare<Thing>(Column::Key,   thing_key);
        IdColumn::declare<Thing>(Column::Name,  thing_name);

        IdColumn::set_defaultList<Thing>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Thing>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_thing_columns();)
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  thing_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Thing>());
        };
    }
}


namespace yq::mithril::displayFN {
    IdColumn::VariantFN  thing_id()
    {
        return [](Id i) -> QVariant {
            Thing    a   = i.as<Thing>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  thing_key()
    {
        return [](Id i) -> QVariant {
            Thing    a   = i.as<Thing>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  thing_name()
    {
        return [](Id i) -> QVariant {
            Thing    a   = i.as<Thing>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}
