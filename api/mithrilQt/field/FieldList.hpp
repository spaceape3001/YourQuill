////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdList.hpp>

namespace yq::mithril {
    class FieldList : public IdListT<Field> {
        Q_OBJECT
    public:
        static constexpr const Column   defColumn   = Column::Key;
    
        FieldList(all_t, QWidget*parent=nullptr);
        FieldList(all_t, Column col, QWidget*parent=nullptr);
        ~FieldList();
    };
}
