////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "group.hpp"
#include <yq/core/DelayInit.hpp>
#include <yq/text/match.hpp>
#include <yq/gluon/core/Utilities.hpp>
#include <yq/mithril/group/GroupCDB.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Group ch)
    {
        static QIcon    qico(":/generic/group.svg");
        if(!ch)
            return QIcon();
        Image   img = cdb::icon(ch);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    group_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::group_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::group_icon();
        ret.fnCompare           = compareFN::group_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    group_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::group_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::group_icon();
        ret.fnCompare           = compareFN::group_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    group_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::group_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::group_icon();
        ret.label               = "Name";
        ret.fnCompare           = compareFN::group_name();
        return ret;
    }

    void    reg_group_columns()
    {
        IdColumn::declare<Group>(Column::Id,    group_id);
        IdColumn::declare<Group>(Column::Key,   group_key);
        IdColumn::declare<Group>(Column::Name,  group_name);

        IdColumn::set_defaultList<Group>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Group>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_group_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN group_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Group>().id, b.as<Group>().id);
        };
    }
    
    IdColumn::CompareFN group_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Group>()), cdb::key(b.as<Group>()));
        };
    }
    
    IdColumn::CompareFN group_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Group>()), cdb::name(b.as<Group>()));
        };
    }
}


namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  group_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Group>());
        };
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN     group_id()
    {
        return [](Id i) -> QVariant {
            Group    a   = i.as<Group>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN     group_key()
    {
        return [](Id i) -> QVariant {
            Group    a   = i.as<Group>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN     group_name()
    {
        return [](Id i) -> QVariant {
            Group    a   = i.as<Group>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}

