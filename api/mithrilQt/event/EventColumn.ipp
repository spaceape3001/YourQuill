////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "EventColumn.hpp"
#include <mithril/event/EventCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    event_id()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::event_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    event_key()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::event_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    event_title()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::event_title();
        ret.label       = "Title";
        return ret;
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN  event_id()
    {
        return [](Id i) -> QVariant {
            Event    a   = i.as<Event>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  event_key()
    {
        return [](Id i) -> QVariant {
            Event    a   = i.as<Event>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  event_title()
    {
        return [](Id i) -> QVariant {
            Event    a   = i.as<Event>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::title(a));
        };
    }
}
