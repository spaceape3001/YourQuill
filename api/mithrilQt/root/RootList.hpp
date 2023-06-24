////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdList.hpp>

namespace yq::mithril {
    class RootList : public IdListT<Root> {
        Q_OBJECT
    public:
        static constexpr const Column   defColumn   = Column::Key;
    
        RootList(all_t, QWidget*parent=nullptr);
        RootList(all_t, Column col, QWidget*parent=nullptr);
        ~RootList();
    };
}
