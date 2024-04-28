////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "drafter.hpp"
#include <0/basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/drafter/DrafterCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Drafter t)
    {
        static QIcon    qico(":/generic/drafter.svg");
        if(!t)
            return QIcon();
        Image   img = cdb::icon(t);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    drafter_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::drafter_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::drafter_icon();
        ret.fnCompare           = compareFN::drafter_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    drafter_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::drafter_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::drafter_icon();
        ret.fnCompare           = compareFN::drafter_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    drafter_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::drafter_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::drafter_icon();
        ret.fnCompare           = compareFN::drafter_name();
        ret.label               = "Name";
        return ret;
    }

    void    reg_drafter_columns()
    {
        IdColumn::declare<Drafter>(Column::Id,    drafter_id);
        IdColumn::declare<Drafter>(Column::Key,   drafter_key);
        IdColumn::declare<Drafter>(Column::Name,  drafter_name);

        IdColumn::set_defaultList<Drafter>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Drafter>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_drafter_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN drafter_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Drafter>().id, b.as<Drafter>().id);
        };
    }
    
    IdColumn::CompareFN drafter_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Drafter>()), cdb::key(b.as<Drafter>()));
        };
    }
    
    IdColumn::CompareFN drafter_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Drafter>()), cdb::name(b.as<Drafter>()));
        };
    }
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  drafter_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Drafter>());
        };
    }
}


namespace yq::mithril::displayFN {
    IdColumn::VariantFN  drafter_id()
    {
        return [](Id i) -> QVariant {
            Drafter    a   = i.as<Drafter>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  drafter_key()
    {
        return [](Id i) -> QVariant {
            Drafter    a   = i.as<Drafter>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  drafter_name()
    {
        return [](Id i) -> QVariant {
            Drafter    a   = i.as<Drafter>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}
