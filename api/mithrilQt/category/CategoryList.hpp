////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdList.hpp>

namespace yq::mithril {
    class CategoryList : public IdListT<Category> {
        Q_OBJECT
    public:
        static constexpr const Column   defColumn   = Column::Key;
    
        CategoryList(all_t, QWidget*parent=nullptr);
        CategoryList(all_t, Column col, QWidget*parent=nullptr);
        ~CategoryList();
    };
}
