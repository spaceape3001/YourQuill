////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LeafColumn.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/leaf/LeafCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image/ImageUtils.hpp>
#include <QIcon>

namespace yq::mithril::column {
    IdColumn    leaf_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::leaf_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::leaf_icon();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    leaf_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::leaf_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::leaf_icon();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    leaf_title(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::leaf_title();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::leaf_icon();
        ret.label               = "Title";
        return ret;
    }

    void    reg_leaf_columns()
    {
        IdColumn::declare<Leaf>(Column::Id,    leaf_id);
        IdColumn::declare<Leaf>(Column::Key,   leaf_key);
        IdColumn::declare<Leaf>(Column::Title, leaf_title);
    }
    
    YQ_INVOKE(reg_leaf_columns();)
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  leaf_icon()
    {
        static QIcon    qico(":/generic/leaf.svg");
        return [](Id i) -> QVariant {
            Leaf   a   = i.as<Leaf>();
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
    IdColumn::VariantFN  leaf_id()
    {
        return [](Id i) -> QVariant {
            Leaf    a   = i.as<Leaf>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  leaf_key()
    {
        return [](Id i) -> QVariant {
            Leaf    a   = i.as<Leaf>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  leaf_title()
    {
        return [](Id i) -> QVariant {
            Leaf    a   = i.as<Leaf>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::title(a));
        };
    }
}
