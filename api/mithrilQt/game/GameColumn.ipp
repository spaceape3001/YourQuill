////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GameColumn.hpp"
#include <mithril/game/GameCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    game_id()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::game_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    game_key()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::game_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    game_title()
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::game_title();
        ret.label       = "Title";
        return ret;
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
