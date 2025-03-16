////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "character.hpp"
#include <yq/core/DelayInit.hpp>
#include <yq/text/match.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/character/CharacterCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Character ch)
    {
        static QIcon    qico(":/generic/character.svg");
        if(!ch)
            return QIcon();
        Image   img = cdb::icon(ch);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    character_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::character_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::character_icon();
        ret.fnCompare           = compareFN::character_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    character_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::character_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::character_icon();
        ret.fnCompare           = compareFN::character_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    character_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::character_name();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::character_icon();
        ret.fnCompare           = compareFN::character_key();
        ret.label               = "Name";
        return ret;
    }

    void    reg_character_columns()
    {
        IdColumn::declare<Character>(Column::Id,    character_id);
        IdColumn::declare<Character>(Column::Key,   character_key);
        IdColumn::declare<Character>(Column::Name,  character_name);

        IdColumn::set_defaultList<Character>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Character>({{ Column::Key, ColOpt::Icon }, Column::Name });
    }
    
    YQ_INVOKE(reg_character_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN character_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Character>().id, b.as<Character>().id);
        };
    }
    
    IdColumn::CompareFN character_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Character>()), cdb::key(b.as<Character>()));
        };
    }
    
    IdColumn::CompareFN character_name()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::name(a.as<Character>()), cdb::name(b.as<Character>()));
        };
    }
}


namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  character_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Character>());
        };
    }
}

namespace yq::mithril::displayFN {
    IdColumn::VariantFN     character_id()
    {
        return [](Id i) -> QVariant {
            Character    a   = i.as<Character>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN     character_key()
    {
        return [](Id i) -> QVariant {
            Character    a   = i.as<Character>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN     character_name()
    {
        return [](Id i) -> QVariant {
            Character    a   = i.as<Character>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::name(a));
        };
    }
}

