////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CharacterList.hpp"
#include "CharacterModel.hpp"

namespace yq::mithril {
    CharacterList::CharacterList(all_t, QWidget*parent) : CharacterList(ALL, defColumn, parent)
    {
    }
    
    CharacterList::CharacterList(all_t, Column col, QWidget*parent) : 
        IdListT<Character>(new CharacterModel(IdModel::Type::List, ALL), parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Characters");
    }

    CharacterList::~CharacterList()
    {
    }
    
    CharacterModel*          CharacterList::model()
    {
        return static_cast<CharacterModel*>(m_model);
    }
    
    const CharacterModel*    CharacterList::model() const
    {
        return static_cast<const CharacterModel*>(m_model);
    }
}

