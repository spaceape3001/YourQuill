////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "meta.hpp"
#include <0/basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/meta/MetaCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Meta l)
    {
        static QIcon    qico(":/generic/meta.svg");
        return qico;
    }
}


namespace yq::mithril::column {
    IdColumn    meta_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::meta_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::meta_icon();
        ret.fnCompare           = compareFN::meta_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    meta_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::meta_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::meta_icon();
        ret.fnCompare           = compareFN::meta_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    meta_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::meta_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::meta_icon();
        ret.fnCompare           = compareFN::meta_name();
        ret.label               = "Name";
        return ret;
    }

    void    reg_meta_columns()
    {
        IdColumn::declare<Meta>(Column::Id,    meta_id);
        IdColumn::declare<Meta>(Column::Key,   meta_key);
        IdColumn::declare<Meta>(Column::Name, meta_name);

        IdColumn::set_defaultList<Meta>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Meta>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_meta_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN meta_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Meta>().id, b.as<Meta>().id);
        };
    }
    
    IdColumn::CompareFN meta_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Meta>()), cdb::key(b.as<Meta>()));
        };
    }
    
    IdColumn::CompareFN meta_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Meta>()), cdb::name(b.as<Meta>()));
        };
    }
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  meta_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Meta>());
        };
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN  meta_id()
    {
        return [](Id i) -> QVariant {
            Meta    a   = i.as<Meta>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  meta_key()
    {
        return [](Id i) -> QVariant {
            Meta    a   = i.as<Meta>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  meta_name()
    {
        return [](Id i) -> QVariant {
            Meta    a   = i.as<Meta>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}
