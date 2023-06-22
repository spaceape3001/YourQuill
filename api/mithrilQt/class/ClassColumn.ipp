////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassColumn.hpp"
#include <mithril/class/ClassCDB.hpp>
#include <mithrilQt/image/ImageUtils.hpp>
#include <gluon/core/Utilities.hpp>
#include <basic/DelayInit.hpp>
#include <QIcon>

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
        static QIcon    qico(":/yq/generic/class.svg");
        return [](Id i) -> QVariant {
            Class   a   = i.as<Class>();
            if(!a)
                return QVariant();
            Image   img = cdb::icon(a);
            if(img)
                return qIcon(img);
            return qico;
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
