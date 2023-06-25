////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "graph.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/graph/GraphCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Graph t)
    {
        static QIcon    qico(":/generic/graph.svg");
        //if(!t)
            //return QIcon();
        //Image   img = cdb::icon(t);
        //if(img)
            //return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    graph_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::graph_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::graph_icon();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    graph_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::graph_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::graph_icon();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    graph_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::graph_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::graph_icon();
        ret.label               = "Name";
        return ret;
    }

    void    reg_graph_columns()
    {
        IdColumn::declare<Graph>(Column::Id,    graph_id);
        IdColumn::declare<Graph>(Column::Key,   graph_key);
        IdColumn::declare<Graph>(Column::Name,  graph_name);

        IdColumn::set_defaultList<Graph>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Graph>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_graph_columns();)
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  graph_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Graph>());
        };
    }
}


namespace yq::mithril::displayFN {
    IdColumn::VariantFN  graph_id()
    {
        return [](Id i) -> QVariant {
            Graph    a   = i.as<Graph>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  graph_key()
    {
        return [](Id i) -> QVariant {
            Graph    a   = i.as<Graph>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  graph_name()
    {
        return [](Id i) -> QVariant {
            Graph    a   = i.as<Graph>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}
