////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "novel.hpp"
#include <0/basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/novel/NovelCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Novel l)
    {
        static QIcon    qico(":/generic/leaf.svg");
        if(!l)
            return QIcon();
        Image   img = cdb::icon(l);
        if(img)
            return qIcon(img);
        return qico;
    }
}


namespace yq::mithril::column {
    IdColumn    novel_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::novel_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::novel_icon();
        ret.fnCompare           = compareFN::novel_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    novel_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::novel_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::novel_icon();
        ret.fnCompare           = compareFN::novel_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    novel_title(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::novel_title();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::novel_icon();
        ret.fnCompare           = compareFN::novel_title();
        ret.label               = "Title";
        return ret;
    }

    void    reg_novel_columns()
    {
        IdColumn::declare<Novel>(Column::Id,    novel_id);
        IdColumn::declare<Novel>(Column::Key,   novel_key);
        IdColumn::declare<Novel>(Column::Title, novel_title);

        IdColumn::set_defaultList<Novel>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Novel>({{ Column::Key, ColOpt::Icon }, Column::Title });
    }
    
    YQ_INVOKE(reg_novel_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN novel_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Novel>().id, b.as<Novel>().id);
        };
    }
    
    IdColumn::CompareFN novel_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Novel>()), cdb::key(b.as<Novel>()));
        };
    }
    
    IdColumn::CompareFN novel_title()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::title(a.as<Novel>()), cdb::title(b.as<Novel>()));
        };
    }
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  novel_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Novel>());
        };
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN  novel_id()
    {
        return [](Id i) -> QVariant {
            Novel    a   = i.as<Novel>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  novel_key()
    {
        return [](Id i) -> QVariant {
            Novel    a   = i.as<Novel>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  novel_title()
    {
        return [](Id i) -> QVariant {
            Novel    a   = i.as<Novel>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::title(a));
        };
    }
}
