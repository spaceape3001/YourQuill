////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "value.hpp"
#include <yq/core/DelayInit.hpp>
#include <yq/text/match.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/value/ValueCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Value t)
    {
        static QIcon    qico(":/generic/value.svg");
        if(!t)
            return QIcon();
        Image   img = cdb::icon(t);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    value_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::value_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::value_icon();
        ret.fnCompare           = compareFN::value_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    value_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::value_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::value_icon();
        ret.fnCompare           = compareFN::value_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    value_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::value_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::value_icon();
        ret.fnCompare           = compareFN::value_name();
        ret.label               = "Name";
        return ret;
    }

    IdColumn    value_value(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::value_value();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::value_icon();
        ret.fnCompare           = compareFN::value_value();
        ret.label               = "Value";
        return ret;
    }

    void    reg_value_columns()
    {
        IdColumn::declare<Value>(Column::Id,    value_id);
        IdColumn::declare<Value>(Column::Key,   value_key);
        IdColumn::declare<Value>(Column::Name,  value_name);
        IdColumn::declare<Value>(Column::Value, value_value);

        IdColumn::set_defaultList<Value>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Value>({{ Column::Key, ColOpt::Icon }, Column::Name, Column::Value });
    }
    
    YQ_INVOKE(reg_value_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN value_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Value>().id, b.as<Value>().id);
        };
    }
    
    IdColumn::CompareFN value_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Value>()), cdb::key(b.as<Value>()));
        };
    }
    
    IdColumn::CompareFN value_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Value>()), cdb::name(b.as<Value>()));
        };
    }

    IdColumn::CompareFN value_value()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::data(a.as<Value>()), cdb::data(b.as<Value>()));
        };
    }
}



namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  value_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Value>());
        };
    }
}


namespace yq::mithril::displayFN {
    IdColumn::VariantFN  value_id()
    {
        return [](Id i) -> QVariant {
            Value    a   = i.as<Value>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  value_key()
    {
        return [](Id i) -> QVariant {
            Value    a   = i.as<Value>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  value_name()
    {
        return [](Id i) -> QVariant {
            Value    a   = i.as<Value>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }

    IdColumn::VariantFN  value_value()
    {
        return [](Id i) -> QVariant {
            Value    a   = i.as<Value>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::data(a));
        };
    }
}
