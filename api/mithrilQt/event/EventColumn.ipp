////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>
#include <mithril/event/EventCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    event_id()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Event    a   = i.as<Event>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
        ret.label   = "ID";
        return ret;
    }
    
    IdColumn    event_key()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Event    a   = i.as<Event>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
        ret.label   = "Key";
        return ret;
    }
    
    IdColumn    event_title()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Event    a   = i.as<Event>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::title(a));
        };
        ret.label   = "Title";
        return ret;
    }
}

