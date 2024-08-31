////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "graphviz.hpp"
#include <yq/basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/graphviz/GraphvizCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Graphviz t)
    {
        static QIcon    qico(":/generic/graphviz.svg");
        //if(!t)
            //return QIcon();
        //Image   img = cdb::icon(t);
        //if(img)
            //return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    graphviz_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::graphviz_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::graphviz_icon();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    graphviz_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::graphviz_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::graphviz_icon();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    graphviz_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::graphviz_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::graphviz_icon();
        ret.label               = "Name";
        return ret;
    }

    void    reg_graphviz_columns()
    {
        IdColumn::declare<Graphviz>(Column::Id,    graphviz_id);
        IdColumn::declare<Graphviz>(Column::Key,   graphviz_key);
        IdColumn::declare<Graphviz>(Column::Name,  graphviz_name);

        IdColumn::set_defaultList<Graphviz>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Graphviz>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_graphviz_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN graphviz_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Graphviz>().id, b.as<Graphviz>().id);
        };
    }
    
    IdColumn::CompareFN graphviz_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Graphviz>()), cdb::key(b.as<Graphviz>()));
        };
    }
    
    IdColumn::CompareFN graphviz_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Graphviz>()), cdb::name(b.as<Graphviz>()));
        };
    }
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  graphviz_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Graphviz>());
        };
    }
}


namespace yq::mithril::displayFN {
    IdColumn::VariantFN  graphviz_id()
    {
        return [](Id i) -> QVariant {
            Graphviz    a   = i.as<Graphviz>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  graphviz_key()
    {
        return [](Id i) -> QVariant {
            Graphviz    a   = i.as<Graphviz>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  graphviz_name()
    {
        return [](Id i) -> QVariant {
            Graphviz    a   = i.as<Graphviz>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}
