////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>

namespace yq::mithril {
    class ClassList : public IdListT<Class> {
        Q_OBJECT
    public:
        static constexpr const Column   defColumn   = Column::Key;
    
        ClassList(all_t, QWidget*parent=nullptr);
        ClassList(all_t, Column col, QWidget*parent=nullptr);
        ~ClassList();
    };
}
