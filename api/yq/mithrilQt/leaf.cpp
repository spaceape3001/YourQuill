////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "leaf.hpp"
#include <yq/core/DelayInit.hpp>
#include <yq/text/match.hpp>
#include <yq/gluon/core/Utilities.hpp>
#include <yq/mithril/leaf/LeafCDB.hpp>
#include <yq/mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Leaf l)
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
    IdColumn    leaf_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::leaf_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::leaf_icon();
        ret.fnCompare           = compareFN::leaf_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    leaf_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::leaf_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::leaf_icon();
        ret.fnCompare           = compareFN::leaf_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    leaf_title(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::leaf_title();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::leaf_icon();
        ret.fnCompare           = compareFN::leaf_title();
        ret.label               = "Title";
        return ret;
    }

    void    reg_leaf_columns()
    {
        IdColumn::declare<Leaf>(Column::Id,    leaf_id);
        IdColumn::declare<Leaf>(Column::Key,   leaf_key);
        IdColumn::declare<Leaf>(Column::Title, leaf_title);

        IdColumn::set_defaultList<Leaf>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Leaf>({{ Column::Key, ColOpt::Icon }, Column::Title });
    }
    
    YQ_INVOKE(reg_leaf_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN leaf_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Leaf>().id, b.as<Leaf>().id);
        };
    }
    
    IdColumn::CompareFN leaf_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Leaf>()), cdb::key(b.as<Leaf>()));
        };
    }
    
    IdColumn::CompareFN leaf_title()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::title(a.as<Leaf>()), cdb::title(b.as<Leaf>()));
        };
    }
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  leaf_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Leaf>());
        };
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN  leaf_id()
    {
        return [](Id i) -> QVariant {
            Leaf    a   = i.as<Leaf>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  leaf_key()
    {
        return [](Id i) -> QVariant {
            Leaf    a   = i.as<Leaf>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  leaf_title()
    {
        return [](Id i) -> QVariant {
            Leaf    a   = i.as<Leaf>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::title(a));
        };
    }
}
