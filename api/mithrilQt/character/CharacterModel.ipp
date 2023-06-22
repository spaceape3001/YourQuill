////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CharacterModel.hpp"
#include "CharacterColumn.hpp"
#include <mithril/character/CharacterProvider.hpp>

namespace yq::mithril {
    CharacterModel::CharacterModel(Type t, all_t, QObject* parent) : 
        CharacterModel(t, Character(), provider::all_characters(), parent)
    {
    }

    CharacterModel::CharacterModel(Type t, Character rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Character>(t, rt, std::move(prov), parent)
    {
    }
    
    CharacterModel::~CharacterModel()
    {
    }
}
