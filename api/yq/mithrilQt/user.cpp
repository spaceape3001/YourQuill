////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "user.hpp"
#include <yq/core/DelayInit.hpp>
#include <yq/text/match.hpp>
#include <yq/gluon/core/Utilities.hpp>
#include <yq/mithril/user/UserCDB.hpp>
#include <yq/mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(User t)
    {
        static QIcon    qico(":/generic/user.svg");
        if(!t)
            return QIcon();
        Image   img = cdb::icon(t);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    user_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::user_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::user_icon();
        ret.fnCompare           = compareFN::user_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    user_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::user_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::user_icon();
        ret.fnCompare           = compareFN::user_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    user_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::user_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::user_icon();
        ret.fnCompare           = compareFN::user_name();
        ret.label               = "Name";
        return ret;
    }

    void    reg_user_columns()
    {
        IdColumn::declare<User>(Column::Id,    user_id);
        IdColumn::declare<User>(Column::Key,   user_key);
        IdColumn::declare<User>(Column::Name,  user_name);

        IdColumn::set_defaultList<User>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<User>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_user_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN user_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<User>().id, b.as<User>().id);
        };
    }
    
    IdColumn::CompareFN user_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<User>()), cdb::key(b.as<User>()));
        };
    }
    
    IdColumn::CompareFN user_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<User>()), cdb::name(b.as<User>()));
        };
    }
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  user_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<User>());
        };
    }
}


namespace yq::mithril::displayFN {
    IdColumn::VariantFN  user_id()
    {
        return [](Id i) -> QVariant {
            User    a   = i.as<User>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  user_key()
    {
        return [](Id i) -> QVariant {
            User    a   = i.as<User>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  user_name()
    {
        return [](Id i) -> QVariant {
            User    a   = i.as<User>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}
