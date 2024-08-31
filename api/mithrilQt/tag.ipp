////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "tag.hpp"
#include <yq/basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
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
        ret.fnCompare           = compareFN::tag_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    tag_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::tag_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::tag_icon();
        ret.fnCompare           = compareFN::tag_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    tag_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::tag_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::tag_icon();
        ret.fnCompare           = compareFN::tag_name();
        ret.label               = "Name";
        return ret;
    }

    void    reg_tag_columns()
    {
        IdColumn::declare<Tag>(Column::Id,    tag_id);
        IdColumn::declare<Tag>(Column::Key,   tag_key);
        IdColumn::declare<Tag>(Column::Name,  tag_name);

        IdColumn::set_defaultList<Tag>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Tag>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_tag_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN tag_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Tag>().id, b.as<Tag>().id);
        };
    }
    
    IdColumn::CompareFN tag_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Tag>()), cdb::key(b.as<Tag>()));
        };
    }
    
    IdColumn::CompareFN tag_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Tag>()), cdb::name(b.as<Tag>()));
        };
    }
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
