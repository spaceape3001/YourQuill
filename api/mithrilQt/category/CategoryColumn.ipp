////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>
#include <mithril/category/CategoryCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    category_id()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Category    a   = i.as<Category>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
        ret.label   = "ID";
        return ret;
    }
    
    IdColumn    category_key()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Category    a   = i.as<Category>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
        ret.label   = "Key";
        return ret;
    }
    
    IdColumn    category_name()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Category    a   = i.as<Category>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
        ret.label   = "Name";
        return ret;
    }
}

