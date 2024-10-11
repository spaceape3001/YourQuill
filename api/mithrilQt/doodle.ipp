////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "doodle.hpp"
#include <yq/core/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/doodle/DoodleCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Doodle t)
    {
        static QIcon    qico(":/generic/doodle.svg");
        if(!t)
            return QIcon();
        Image   img = cdb::icon(t);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    doodle_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::doodle_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::doodle_icon();
        ret.fnCompare           = compareFN::doodle_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    doodle_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::doodle_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::doodle_icon();
        ret.fnCompare           = compareFN::doodle_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    doodle_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::doodle_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::doodle_icon();
        ret.fnCompare           = compareFN::doodle_name();
        ret.label               = "Name";
        return ret;
    }

    void    reg_doodle_columns()
    {
        IdColumn::declare<Doodle>(Column::Id,    doodle_id);
        IdColumn::declare<Doodle>(Column::Key,   doodle_key);
        IdColumn::declare<Doodle>(Column::Name,  doodle_name);

        IdColumn::set_defaultList<Doodle>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Doodle>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_doodle_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN doodle_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Doodle>().id, b.as<Doodle>().id);
        };
    }
    
    IdColumn::CompareFN doodle_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Doodle>()), cdb::key(b.as<Doodle>()));
        };
    }
    
    IdColumn::CompareFN doodle_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Doodle>()), cdb::name(b.as<Doodle>()));
        };
    }
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  doodle_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Doodle>());
        };
    }
}


namespace yq::mithril::displayFN {
    IdColumn::VariantFN  doodle_id()
    {
        return [](Id i) -> QVariant {
            Doodle    a   = i.as<Doodle>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  doodle_key()
    {
        return [](Id i) -> QVariant {
            Doodle    a   = i.as<Doodle>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  doodle_name()
    {
        return [](Id i) -> QVariant {
            Doodle    a   = i.as<Doodle>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}
