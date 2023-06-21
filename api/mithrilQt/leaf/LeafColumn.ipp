////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LeafColumn.hpp"
#include <mithril/leaf/LeafCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    leaf_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::leaf_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    leaf_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::leaf_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    leaf_title(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::leaf_title();
        ret.label       = "Title";
        return ret;
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
