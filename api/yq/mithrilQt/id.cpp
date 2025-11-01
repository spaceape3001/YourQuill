////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "id.hpp"
#include <yq/text/match.hpp>
#include <yq/gluon/core/Utilities.hpp>
#include <QIcon>

using namespace yq::gluon;

namespace yq::mithril::column {
    IdColumn                id_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::id_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::id_icon();
        ret.fnCompare           = compareFN::id_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn                id_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::id_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::id_icon();
        ret.fnCompare           = compareFN::id_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn                id_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::id_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::id_icon();
        ret.fnCompare           = compareFN::id_name();
        ret.label               = "Name";
        return ret;
    }

    IdColumn                id_type(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::id_type();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::id_icon();
        ret.fnCompare           = compareFN::id_type();
        ret.label               = "Type";
        return ret;
    }
}


namespace yq::mithril::compareFN {
    IdColumn::CompareFN id_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.id(), b.id());
        };
    }
    
    IdColumn::CompareFN id_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(a.key(), b.key());
        };
    }
    
    IdColumn::CompareFN id_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(a.name(), b.name());
        };
    }

    IdColumn::CompareFN id_type()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(a.type_name(), b.type_name());
        };
    }
}


namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  id_icon()
    {
        return [](Id i) -> QVariant {
            return id_qIcon(i);
        };
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN     id_id()
    {
        return [](Id i) -> QVariant {
            return (quint64) i.id();
        };
    }
    
    IdColumn::VariantFN     id_key()
    {
        return [](Id i) -> QVariant {
            return qString(i.key());
        };
    }

    IdColumn::VariantFN     id_name()
    {
        return [](Id i) -> QVariant {
            return qString(i.name());
        };
    }

    IdColumn::VariantFN     id_type()
    {
        return [](Id i) -> QVariant {
            return qString(Id::type_name(i.type()));
        };
    }
}
