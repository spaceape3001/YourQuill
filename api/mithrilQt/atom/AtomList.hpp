////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdList.hpp>

namespace yq::mithril {
    class AtomModel;
    class AtomList : public IdListT<Atom> {
        Q_OBJECT
    public:
        static constexpr const Column   defColumn   = Column::Key;
    
        AtomList(all_t, QWidget*parent=nullptr);
        AtomList(all_t, Column col, QWidget*parent=nullptr);
        ~AtomList();
    };
}
