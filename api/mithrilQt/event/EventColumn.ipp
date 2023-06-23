////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "EventColumn.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/event/EventCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image/ImageUtils.hpp>
#include <QIcon>

namespace yq::mithril::column {
    IdColumn    event_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::event_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::event_icon();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    event_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::event_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::event_icon();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    event_title(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::event_title();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::event_icon();
        ret.label               = "Title";
        return ret;
    }

    void    reg_event_columns()
    {
        IdColumn::declare<Event>(Column::Id,    event_id);
        IdColumn::declare<Event>(Column::Key,   event_key);
        IdColumn::declare<Event>(Column::Title, event_title);
    }
    
    YQ_INVOKE(reg_event_columns();)
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  event_icon()
    {
        static QIcon    qico(":/generic/event.svg");
        return [](Id i) -> QVariant {
            Event   a   = i.as<Event>();
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
