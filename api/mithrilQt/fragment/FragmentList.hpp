////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdList.hpp>

namespace yq::mithril {
    class FragmentList : public IdListT<Fragment> {
        Q_OBJECT
    public:
        static constexpr const Column   defColumn   = Column::Key;
    
        FragmentList(all_t, QWidget*parent=nullptr);
        FragmentList(all_t, Column col, QWidget*parent=nullptr);
        ~FragmentList();
        
    };
}
