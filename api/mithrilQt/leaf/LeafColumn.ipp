////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>
#include <mithril/leaf/LeafCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    leaf_id()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Leaf    a   = i.as<Leaf>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
        ret.label   = "ID";
        return ret;
    }
    
    IdColumn    leaf_key()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Leaf    a   = i.as<Leaf>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
        ret.label   = "Key";
        return ret;
    }
    
    IdColumn    leaf_title()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Leaf    a   = i.as<Leaf>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::title(a));
        };
        ret.label   = "Title";
        return ret;
    }
}

