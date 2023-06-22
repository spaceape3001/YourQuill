////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GameColumn.hpp"
#include <basic/DelayInit.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/game/GameCDB.hpp>

namespace yq::mithril::column {
    IdColumn    game_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::game_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    game_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::game_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    game_title(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::game_title();
        ret.label       = "Title";
        return ret;
    }

    void    reg_game_columns()
    {
        IdColumn::declare<Game>(Column::Id,    game_id);
        IdColumn::declare<Game>(Column::Key,   game_key);
        IdColumn::declare<Game>(Column::Title, game_title);
    }
    
    YQ_INVOKE(reg_game_columns();)
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
