////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>
#include <mithril/atom/AtomCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    atom_id()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Atom    a   = i.as<Atom>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
        ret.label   = "ID";
        return ret;
    }
    
    IdColumn    atom_key()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Atom    a   = i.as<Atom>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
        ret.label   = "Key";
        return ret;
    }
    
    IdColumn    atom_name()
    {
        IdColumn    ret;
        ret.fnDisplay   = [](Id i) -> QVariant {
            Atom    a   = i.as<Atom>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
        ret.label   = "Name";
        return ret;
    }
}

