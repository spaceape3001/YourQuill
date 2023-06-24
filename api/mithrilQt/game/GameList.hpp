////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdList.hpp>

namespace yq::mithril {
    class GameList : public IdListT<Game> {
        Q_OBJECT
    public:
        static constexpr const Column   defColumn   = Column::Key;
    
        GameList(all_t, QWidget*parent=nullptr);
        GameList(all_t, Column col, QWidget*parent=nullptr);
        ~GameList();
    };
}
