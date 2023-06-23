////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CharacterTable.hpp"
#include "CharacterModel.hpp"

namespace yq::mithril {
    std::span<const ColumnSpec>  CharacterTable::defColumns() 
    {
        static ColumnSpec   s_data[] = { { Column::Key, ColOpt::Icon}, Column::Name };
        return std::span<const ColumnSpec>(std::begin(s_data), std::end(s_data));
    }

    CharacterTable::CharacterTable(all_t, QWidget*parent) : CharacterTable(ALL, defColumns(), parent)
    {
    }
    
    CharacterTable::CharacterTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent) : 
        CharacterTable(ALL, span(columns), parent)
    {
    }
    
    CharacterTable::CharacterTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent) : 
        IdTableT<Character>(new CharacterModel(IdModel::Type::Table, ALL), parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Characters");
    }

    CharacterTable::~CharacterTable()
    {
    }
    
    
    CharacterModel*          CharacterTable::model()
    {
        return static_cast<CharacterModel*>(m_model);
    }
    
    const CharacterModel*    CharacterTable::model() const
    {
        return static_cast<const CharacterModel*>(m_model);
    }
}

