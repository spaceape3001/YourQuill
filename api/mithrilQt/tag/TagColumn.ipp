////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagColumn.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/tag/TagCDB.hpp>

namespace yq::mithril::column {
    IdColumn    tag_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::tag_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    tag_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::tag_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    tag_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::tag_name();
        ret.label       = "Name";
        return ret;
    }

    void    reg_tag_columns()
    {
        IdColumn::declare<Tag>(Column::Id,    tag_id);
        IdColumn::declare<Tag>(Column::Key,   tag_key);
        IdColumn::declare<Tag>(Column::Name,  tag_name);
    }
    
    YQ_INVOKE(reg_tag_columns();)
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN  tag_id()
    {
        return [](Id i) -> QVariant {
            Tag    a   = i.as<Tag>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  tag_key()
    {
        return [](Id i) -> QVariant {
            Tag    a   = i.as<Tag>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  tag_name()
    {
        return [](Id i) -> QVariant {
            Tag    a   = i.as<Tag>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}
