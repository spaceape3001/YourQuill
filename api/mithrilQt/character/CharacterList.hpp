////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdList.hpp>

namespace yq::mithril {
    class CharacterList : public IdListT<Character> {
        Q_OBJECT
    public:
        static constexpr const Column   defColumn   = Column::Key;
    
        CharacterList(all_t, QWidget*parent=nullptr);
        CharacterList(all_t, Column col, QWidget*parent=nullptr);
        ~CharacterList();
    };
}
