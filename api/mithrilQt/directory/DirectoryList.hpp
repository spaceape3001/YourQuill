////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdList.hpp>

namespace yq::mithril {
    class DirectoryList : public IdListT<Directory> {
        Q_OBJECT
    public:
        static constexpr const Column   defColumn   = Column::Key;
    
        DirectoryList(all_t, QWidget*parent=nullptr);
        DirectoryList(all_t, Column col, QWidget*parent=nullptr);
        ~DirectoryList();
    };
}
