////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CategoryColumn.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/category/CategoryCDB.hpp>

namespace yq::mithril::column {
    IdColumn    category_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::category_id();
        ret.label   = "ID";
        return ret;
    }
    
    IdColumn    category_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::category_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    category_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::category_name();
        ret.label       = "Name";
        return ret;
    }

    void    reg_category_columns()
    {
        IdColumn::declare<Category>(Column::Id,    category_id);
        IdColumn::declare<Category>(Column::Key,   category_key);
        IdColumn::declare<Category>(Column::Name,  category_name);
    }
    
    YQ_INVOKE(reg_category_columns();)
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
