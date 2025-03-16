////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "class.hpp"
#include <yq/core/DelayInit.hpp>
#include <yq/text/match.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon qIcon(Class cls)
    {
        static QIcon    qico(":/generic/class.svg");
        if(!cls)
            return QIcon();
        Image   img = cdb::icon(cls);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    class_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::class_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::class_icon();
        ret.fnCompare           = compareFN::class_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    class_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::class_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::class_icon();
        ret.fnCompare           = compareFN::class_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    class_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::class_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::class_icon();
        ret.fnCompare           = compareFN::class_name();
        ret.label               = "Name";
        return ret;
    }
    
    void    reg_class_columns()
    {
        IdColumn::declare<Class>(Column::Id,    class_id);
        IdColumn::declare<Class>(Column::Key,   class_key);
        IdColumn::declare<Class>(Column::Name,  class_name);

        IdColumn::set_defaultList<Class>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Class>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_class_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN class_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Class>().id, b.as<Class>().id);
        };
    }
    
    IdColumn::CompareFN class_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Class>()), cdb::key(b.as<Class>()));
        };
    }
    
    IdColumn::CompareFN class_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Class>()), cdb::name(b.as<Class>()));
        };
    }
}


namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  class_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Class>());
        };
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN  class_id()
    {
        return [](Id i) -> QVariant {
            Class    a   = i.as<Class>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  class_key()
    {
        return [](Id i) -> QVariant {
            Class    a   = i.as<Class>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  class_name()
    {
        return [](Id i) -> QVariant {
            Class    a   = i.as<Class>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
    
}
