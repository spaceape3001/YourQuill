////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CharacterColumn.hpp"
#include <mithril/character/CharacterCDB.hpp>
#include <gluon/core/Utilities.hpp>

namespace yq::mithril::column {
    IdColumn    character_id(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::character_id();
        ret.label       = "ID";
        return ret;
    }
    
    IdColumn    character_key(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::character_key();
        ret.label       = "Key";
        return ret;
    }
    
    IdColumn    character_name(ColOpts opts)
    {
        IdColumn    ret;
        ret.fnDisplay   = displayFN::character_name();
        ret.label       = "Name";
        return ret;
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

