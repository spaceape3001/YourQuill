////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "event.hpp"
#include <yq/core/DelayInit.hpp>
#include <yq/text/match.hpp>
#include <yq/gluon/core/Utilities.hpp>
#include <yq/mithril/event/EventCDB.hpp>
#include <yq/mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Event evt)
    {
        static QIcon    qico(":/generic/event.svg");
        if(!evt)
            return QIcon();
        Image   img = cdb::icon(evt);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    event_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::event_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::event_icon();
        ret.fnCompare           = compareFN::event_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    event_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::event_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::event_icon();
        ret.fnCompare           = compareFN::event_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    event_title(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::event_title();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::event_icon();
        ret.fnCompare           = compareFN::event_title();
        ret.label               = "Title";
        return ret;
    }

    void    reg_event_columns()
    {
        IdColumn::declare<Event>(Column::Id,    event_id);
        IdColumn::declare<Event>(Column::Key,   event_key);
        IdColumn::declare<Event>(Column::Title, event_title);

        IdColumn::set_defaultList<Event>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Event>({{ Column::Key, ColOpt::Icon }, Column::Title });
    }
    
    YQ_INVOKE(reg_event_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN event_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Event>().id, b.as<Event>().id);
        };
    }
    
    IdColumn::CompareFN event_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Event>()), cdb::key(b.as<Event>()));
        };
    }
    
    IdColumn::CompareFN event_title()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::title(a.as<Event>()), cdb::title(b.as<Event>()));
        };
    }
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  event_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Event>());
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
