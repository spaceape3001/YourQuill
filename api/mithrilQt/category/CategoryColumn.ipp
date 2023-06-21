////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CategoryColumn.hpp"
#include <mithril/category/CategoryCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    category_id()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::category_id();
        ret.label   = "ID";
        return ret;
    }
    
    IdColumn    category_key()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::category_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    category_name()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::category_name();
        ret.label       = "Name";
        return ret;
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN    category_id()
    {
        return [](Id i) -> QVariant {
            Category    a   = i.as<Category>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN    category_key()
    {
        return [](Id i) -> QVariant {
            Category    a   = i.as<Category>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN    category_name()
    {
        return [](Id i) -> QVariant {
            Category    a   = i.as<Category>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}
