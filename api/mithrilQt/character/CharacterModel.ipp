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
    std::span<const Column>  CharacterModel::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Name };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    CharacterModel::CharacterModel(Type t, all_t, std::span<const Column> cols, QObject* parent) : 
        CharacterModel(t, Character(), provider::all_characters(), cols, parent)
    {
    }

    CharacterModel::CharacterModel(Type t, Character rt, IdProvider&& prov, std::span<const Column> cols, QObject*parent) : 
        IdModelT<Character>(t, rt, std::move(prov), parent)
    {
        for(Column c : cols)
            addColumn(c);
        reload();
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
    
}
