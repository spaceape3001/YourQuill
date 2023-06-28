////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "category.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/category/CategoryCDB.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Category cat)
    {
        static QIcon    qico(":/generic/category.svg");
        if(!cat)
            return QIcon();
        Image   img = cdb::icon(cat);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    category_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::category_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::category_icon();
        ret.fnCompare           = compareFN::category_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    category_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::category_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::category_icon();
        ret.fnCompare           = compareFN::category_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    category_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::category_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::category_icon();
        ret.fnCompare           = compareFN::category_name();
        ret.label               = "Name";
        return ret;
    }

    void    reg_category_columns()
    {
        IdColumn::declare<Category>(Column::Id,    category_id);
        IdColumn::declare<Category>(Column::Key,   category_key);
        IdColumn::declare<Category>(Column::Name,  category_name);

        IdColumn::set_defaultList<Category>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Category>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_category_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN category_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Category>().id, b.as<Category>().id);
        };
    }
    
    IdColumn::CompareFN category_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Category>()), cdb::key(b.as<Category>()));
        };
    }
    
    IdColumn::CompareFN category_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Category>()), cdb::name(b.as<Category>()));
        };
    }
}



namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  category_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Category>());
        };
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
