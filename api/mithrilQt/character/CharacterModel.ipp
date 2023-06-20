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
    
    void    CharacterModel::addColumn(Column col)
    {
        switch(col){
        case Column::Id:
            addColumn(column::character_id());
            break;
        case Column::Key:
            addColumn(column::character_key());
            break;
        case Column::Name:
            addColumn(column::character_name());
            break;
        default:
            break;
        }
    }
    
    void    CharacterModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }
}
