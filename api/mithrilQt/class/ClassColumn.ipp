////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassColumn.hpp"
#include <mithril/class/ClassCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    class_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::class_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    class_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::class_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    class_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::class_name();
        ret.label       = "Name";
        return ret;
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
