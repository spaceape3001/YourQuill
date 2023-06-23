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
#include <mithril/image/Image.hpp>
#include <mithrilQt/image/ImageUtils.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Tag t)
    {
        static QIcon    qico(":/generic/tag.svg");
        if(!t)
            return QIcon();
        Image   img = cdb::icon(t);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    tag_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::tag_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::tag_icon();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    tag_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::tag_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::tag_icon();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    tag_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::tag_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::tag_icon();
        ret.label               = "Name";
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

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  tag_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Tag>());
        };
    }
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
