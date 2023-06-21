////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/AtomCDB.hpp>
#include <gluon/core/Utilities.hpp>
#include "AtomColumn.hpp"

namespace yq::mithril::column {
    IdColumn    atom_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::atom_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    atom_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::atom_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    atom_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::atom_name();
        ret.label       = "Name";
        return ret;
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN     atom_id()
    {
        return [](Id i) -> QVariant {
            Atom    a   = i.as<Atom>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN     atom_key()
    {
        return [](Id i) -> QVariant {
            Atom    a   = i.as<Atom>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN     atom_name()
    {
        return [](Id i) -> QVariant {
            Atom    a   = i.as<Atom>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
    
}

