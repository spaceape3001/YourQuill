////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "game.hpp"
#include <yq/core/DelayInit.hpp>
#include <yq/text/match.hpp>
#include <yq/gluon/core/Utilities.hpp>
#include <yq/mithril/game/GameCDB.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithrilQt/image.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon   qIcon(Game g)
    {
        static QIcon    qico(":/generic/game.svg");
        if(!g)
            return QIcon();
        Image   img = cdb::icon(g);
        if(img)
            return qIcon(img);
        return qico;
    }
}

namespace yq::mithril::column {
    IdColumn    game_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::game_id();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::game_icon();
        ret.fnCompare           = compareFN::game_id();
        ret.label               = "ID";
        return ret;
    }
    
    IdColumn    game_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::game_key();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::game_icon();
        ret.fnCompare           = compareFN::game_key();
        ret.label               = "Key";
        return ret;
    }
    
    IdColumn    game_title(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay           = displayFN::game_title();
        if(opts[ColOpt::Icon])
            ret.fnDecoration    = decorationFN::game_icon();
        ret.fnCompare           = compareFN::game_title();
        ret.label               = "Title";
        return ret;
    }

    void    reg_game_columns()
    {
        IdColumn::declare<Game>(Column::Id,    game_id);
        IdColumn::declare<Game>(Column::Key,   game_key);
        IdColumn::declare<Game>(Column::Title, game_title);

        IdColumn::set_defaultList<Game>({ Column::Key, ColOpt::Icon });
        IdColumn::set_defaultTable<Game>({{ Column::Key, ColOpt::Icon }, Column::Title });
    }
    
    YQ_INVOKE(reg_game_columns();)
}

namespace yq::mithril::compareFN {
    IdColumn::CompareFN game_id()
    {
        return [](Id a, Id b) -> Compare {
            return compare(a.as<Game>().id, b.as<Game>().id);
        };
    }
    
    IdColumn::CompareFN game_key()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::key(a.as<Game>()), cdb::key(b.as<Game>()));
        };
    }
    
    IdColumn::CompareFN game_title()
    {
        return [](Id a, Id b) -> Compare {
            return compare_igCase(cdb::title(a.as<Game>()), cdb::title(b.as<Game>()));
        };
    }
}

namespace yq::mithril::decorationFN {
    IdColumn::VariantFN  game_icon()
    {
        return [](Id i) -> QVariant {
            return qIcon(i.as<Game>());
        };
    }
}


namespace yq::mithril::displayFN {
    IdColumn::VariantFN  game_id()
    {
        return [](Id i) -> QVariant {
            Game    a   = i.as<Game>();
            if(!a)
                return QVariant();
            return (quint64) a.id;
        };
    }
    
    IdColumn::VariantFN  game_key()
    {
        return [](Id i) -> QVariant {
            Game    a   = i.as<Game>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::key(a));
        };
    }
    
    IdColumn::VariantFN  game_title()
    {
        return [](Id i) -> QVariant {
            Game    a   = i.as<Game>();
            if(!a)
                return QVariant();
            return gluon::qString(cdb::title(a));
        };
    }
}
