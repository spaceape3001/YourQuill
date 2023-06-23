////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AtomColumn.hpp"

#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/atom/AtomCDB.hpp>
#include <mithrilQt/image/ImageUtils.hpp>
#include <QIcon>

namespace yq::mithril::column {
    IdColumn    atom_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::atom_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::atom_icon();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    atom_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::atom_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::atom_icon();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    atom_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::atom_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::atom_icon();
        ret.label               = "Name";
        return ret;
    }

    void    reg_atom_columns()
    {
        IdColumn::declare<Atom>(Column::Id,    atom_id);
        IdColumn::declare<Atom>(Column::Key,   atom_key);
        IdColumn::declare<Atom>(Column::Name,  atom_name);
    }
    
    YQ_INVOKE(reg_atom_columns();)
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  atom_icon()
    {
        static QIcon    qico(":/generic/atom.svg");
        return [](Id i) -> QVariant {
            Atom   a   = i.as<Atom>();
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

