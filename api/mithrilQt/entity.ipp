////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "entity.hpp"
#include <0/basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/entity/EntityCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Entity ch)
    {
        static QIcon    qico(":/generic/entity.svg");
        if(!ch)
            return QIcon();
        Image   img = cdb::icon(ch);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    entity_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::entity_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::entity_icon();
        ret.fnCompare           = compareFN::entity_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    entity_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::entity_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::entity_icon();
        ret.fnCompare           = compareFN::entity_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    entity_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::entity_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::entity_icon();
        ret.fnCompare           = compareFN::entity_name();
        ret.label               = "Name";
        return ret;
    }

    void    reg_entity_columns()
    {
        IdColumn::declare<Entity>(Column::Id,    entity_id);
        IdColumn::declare<Entity>(Column::Key,   entity_key);
        IdColumn::declare<Entity>(Column::Name,  entity_name);

        IdColumn::set_defaultList<Entity>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Entity>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_entity_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN entity_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Entity>().id, b.as<Entity>().id);
        };
    }
    
    IdColumn::CompareFN entity_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Entity>()), cdb::key(b.as<Entity>()));
        };
    }
    
    IdColumn::CompareFN entity_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Entity>()), cdb::name(b.as<Entity>()));
        };
    }
}


namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  entity_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Entity>());
        };
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN     entity_id()
    {
        return [](Id i) -> QVariant {
            Entity    a   = i.as<Entity>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN     entity_key()
    {
        return [](Id i) -> QVariant {
            Entity    a   = i.as<Entity>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN     entity_name()
    {
        return [](Id i) -> QVariant {
            Entity    a   = i.as<Entity>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}

