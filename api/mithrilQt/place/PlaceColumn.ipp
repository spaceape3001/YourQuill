////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PlaceColumn.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/place/PlaceCDB.hpp>

namespace yq::mithril::column {
    IdColumn    place_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::place_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    place_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::place_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    place_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::place_name();
        ret.label       = "Name";
        return ret;
    }

    void    reg_place_columns()
    {
        IdColumn::declare<Place>(Column::Id,    place_id);
        IdColumn::declare<Place>(Column::Key,   place_key);
        IdColumn::declare<Place>(Column::Name,  place_name);
    }
    
    YQ_INVOKE(reg_place_columns();)
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
