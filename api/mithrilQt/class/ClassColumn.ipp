////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassColumn.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image/ImageUtils.hpp>
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
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    class_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::class_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::class_icon();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    class_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::class_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::class_icon();
        ret.label               = "Name";
        return ret;
    }
    
    void    reg_class_columns()
    {
        IdColumn::declare<Class>(Column::Id,    class_id);
        IdColumn::declare<Class>(Column::Key,   class_key);
        IdColumn::declare<Class>(Column::Name,  class_name);
    }
    
    YQ_INVOKE(reg_class_columns();)
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
