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
    std::optional<IdColumn>     CharacterModel::resolve(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            return column::character_id(opts);
        case Column::Key:
            return column::character_key(opts);
        case Column::Name:
            return column::character_name(opts);
        default:
            return {};
        }
    }

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
    
    void    CharacterModel::addColumn(Column col, ColOpts opts)
    {
        auto    cc  = resolve(col, opts);
        if(cc)
            addColumn(std::move(*cc));
    }
    
    void    CharacterModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }

    void    CharacterModel::setColumn(Column col, ColOpts opts)
    {
        auto    cc = resolve(col, opts);
        if(cc)
            setColumn(std::move(*cc));
    }
}
