////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "atom.hpp"

#include <yq/core/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/atom/AtomCDB.hpp>
#include <mithrilQt/field.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>
#include <yq/text/match.hpp>

namespace yq::mithril {
    QIcon   qIcon(Atom a)
    {
        static QIcon    qico(":/generic/atom.svg");
        if(!a)
            return QIcon();
        Image   img = cdb::icon(a);
        if(img)
            return qIcon(img);
        return qico;
    }

    QIcon   qIcon(Atom::Property p)
    {
        return qIcon(cdb::field(p));
    }
}

namespace yq::mithril::column {
    IdColumn    atom_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::atom_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::atom_icon();
        ret.fnCompare           = compareFN::atom_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    atom_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::atom_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::atom_icon();
        ret.fnCompare           = compareFN::atom_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    atom_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::atom_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::atom_icon();
        ret.fnCompare           = compareFN::atom_name();
        ret.label               = "Name";
        return ret;
    }

    void    reg_atom()
    {
        IdColumn::declare<Atom>(Column::Id,    atom_id);
        IdColumn::declare<Atom>(Column::Key,   atom_key);
        IdColumn::declare<Atom>(Column::Name,  atom_name);

        IdColumn::set_defaultList<Atom>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Atom>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_atom();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN atom_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Atom>().id, b.as<Atom>().id);
        };
    }
    
    IdColumn::CompareFN atom_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Atom>()), cdb::key(b.as<Atom>()));
        };
    }
    
    IdColumn::CompareFN atom_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Atom>()), cdb::name(b.as<Atom>()));
        };
    }
}


namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  atom_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Atom>());
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

