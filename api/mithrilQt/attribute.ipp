////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "attribute.hpp"

#include <yq/basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/attribute/AttributeCDB.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Attribute a)
    {
        static QIcon    qico(":/generic/attribute.svg");
        //if(!a)
            //return QIcon();
        //Image   img = cdb::icon(a);
        //if(img)
            //return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    attribute_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::attribute_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::attribute_icon();
        ret.fnCompare           = compareFN::attribute_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    attribute_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::attribute_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::attribute_icon();
        ret.fnCompare           = compareFN::attribute_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    attribute_value(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::attribute_value();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::attribute_icon();
        ret.fnCompare           = compareFN::attribute_value();
        ret.label               = "Value";
        return ret;
    }

    void    reg_attribute()
    {
        IdColumn::declare<Attribute>(Column::Id,        attribute_id);
        IdColumn::declare<Attribute>(Column::Key,       attribute_key);
        IdColumn::declare<Attribute>(Column::Value,     attribute_value);

        IdColumn::set_defaultList<Attribute>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Attribute>({{ Column::Key, ColOpt::Icon }, Column::Value});
    }
    
    YQ_INVOKE(reg_attribute();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN attribute_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Attribute>().id, b.as<Attribute>().id);
        };
    }
    
    IdColumn::CompareFN attribute_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Attribute>()), cdb::key(b.as<Attribute>()));
        };
    }
    
    IdColumn::CompareFN attribute_value()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::value(a.as<Attribute>()), cdb::value(b.as<Attribute>()));
        };
    }
}


namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  attribute_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Attribute>());
        };
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN     attribute_id()
    {
        return [](Id i) -> QVariant {
            Attribute    a   = i.as<Attribute>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN     attribute_key()
    {
        return [](Id i) -> QVariant {
            Attribute    a   = i.as<Attribute>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN     attribute_value()
    {
        return [](Id i) -> QVariant {
            Attribute    a   = i.as<Attribute>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::value(a));
        };
    }
    
}

