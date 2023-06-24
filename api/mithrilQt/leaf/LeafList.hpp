////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdList.hpp>

namespace yq::mithril {
    class LeafList : public IdListT<Leaf> {
        Q_OBJECT
    public:
        static constexpr const Column   defColumn   = Column::Key;
    
        LeafList(all_t, QWidget*parent=nullptr);
        LeafList(all_t, Column col, QWidget*parent=nullptr);
        ~LeafList();
    };
}
