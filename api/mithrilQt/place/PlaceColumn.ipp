////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PlaceColumn.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/place/PlaceCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image/ImageUtils.hpp>
#include <QIcon>

namespace yq::mithril::column {
    IdColumn    place_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::place_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::place_icon();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    place_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::place_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::place_icon();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    place_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::place_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::place_icon();
        ret.label               = "Name";
        return ret;
    }

    void    reg_place_columns()
    {
        IdColumn::declare<Place>(Column::Id,    place_id);
        IdColumn::declare<Place>(Column::Key,   place_key);
        IdColumn::declare<Place>(Column::Name,  place_name);
    }
    
    YQ_INVOKE(reg_place_columns();)
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  place_icon()
    {
        static QIcon    qico(":/generic/place.svg");
        return [](Id i) -> QVariant {
            Place   a   = i.as<Place>();
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
    IdColumn::VariantFN  place_id()
    {
        return [](Id i) -> QVariant {
            Place    a   = i.as<Place>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  place_key()
    {
        return [](Id i) -> QVariant {
            Place    a   = i.as<Place>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  place_name()
    {
        return [](Id i) -> QVariant {
            Place    a   = i.as<Place>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}
