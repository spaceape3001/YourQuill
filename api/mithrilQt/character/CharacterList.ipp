////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CharacterList.hpp"

namespace yq::mithril {
    CharacterList::CharacterList(all_t, QWidget*parent) : CharacterList(ALL, defColumn, parent)
    {
    }
    
    CharacterList::CharacterList(all_t, Column col, QWidget*parent) : 
        IdListT<Character>(ALL, parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Characters");
    }

    CharacterList::~CharacterList()
    {
    }
}

