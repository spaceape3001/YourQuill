////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "place.hpp"
#include <yq/basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/place/PlaceCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Place p)
    {
        static QIcon    qico(":/generic/place.svg");
        if(!p)
            return QIcon();
        Image   img = cdb::icon(p);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    place_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::place_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::place_icon();
        ret.fnCompare           = compareFN::place_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    place_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::place_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::place_icon();
        ret.fnCompare           = compareFN::place_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    place_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::place_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::place_icon();
        ret.fnCompare           = compareFN::place_name();
        ret.label               = "Name";
        return ret;
    }

    void    reg_place_columns()
    {
        IdColumn::declare<Place>(Column::Id,    place_id);
        IdColumn::declare<Place>(Column::Key,   place_key);
        IdColumn::declare<Place>(Column::Name,  place_name);

        IdColumn::set_defaultList<Place>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Place>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_place_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN place_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Place>().id, b.as<Place>().id);
        };
    }
    
    IdColumn::CompareFN place_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Place>()), cdb::key(b.as<Place>()));
        };
    }
    
    IdColumn::CompareFN place_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Place>()), cdb::name(b.as<Place>()));
        };
    }
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  place_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Place>());
        };
    }
}


namespace yq::mithril::displayFN {
    IdColumn::VariantFN  place_id()
    {
        return [](Id i) -> QVariant {
            Place    a   = i.as<Place>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  place_key()
    {
        return [](Id i) -> QVariant {
            Place    a   = i.as<Place>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  place_name()
    {
        return [](Id i) -> QVariant {
            Place    a   = i.as<Place>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}
