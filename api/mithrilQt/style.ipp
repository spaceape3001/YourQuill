////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "style.hpp"
#include <yq/basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/style/StyleCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Style t)
    {
        static QIcon    qico(":/generic/style.svg");
        if(!t)
            return QIcon();
        Image   img = cdb::icon(t);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    style_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::style_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::style_icon();
        ret.fnCompare           = compareFN::style_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    style_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::style_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::style_icon();
        ret.fnCompare           = compareFN::style_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    style_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::style_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::style_icon();
        ret.fnCompare           = compareFN::style_name();
        ret.label               = "Name";
        return ret;
    }

    void    reg_style_columns()
    {
        IdColumn::declare<Style>(Column::Id,    style_id);
        IdColumn::declare<Style>(Column::Key,   style_key);
        IdColumn::declare<Style>(Column::Name,  style_name);

        IdColumn::set_defaultList<Style>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Style>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_style_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN style_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Style>().id, b.as<Style>().id);
        };
    }
    
    IdColumn::CompareFN style_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Style>()), cdb::key(b.as<Style>()));
        };
    }
    
    IdColumn::CompareFN style_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Style>()), cdb::name(b.as<Style>()));
        };
    }
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  style_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Style>());
        };
    }
}


namespace yq::mithril::displayFN {
    IdColumn::VariantFN  style_id()
    {
        return [](Id i) -> QVariant {
            Style    a   = i.as<Style>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  style_key()
    {
        return [](Id i) -> QVariant {
            Style    a   = i.as<Style>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  style_name()
    {
        return [](Id i) -> QVariant {
            Style    a   = i.as<Style>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}
