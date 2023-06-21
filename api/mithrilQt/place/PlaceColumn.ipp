////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PlaceColumn.hpp"
#include <mithril/place/PlaceCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    place_id()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::place_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    place_key()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::place_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    place_name()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::place_name();
        ret.label       = "Name";
        return ret;
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
