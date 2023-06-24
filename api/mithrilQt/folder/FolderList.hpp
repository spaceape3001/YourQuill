////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdList.hpp>

namespace yq::mithril {
    class FolderList : public IdListT<Folder> {
        Q_OBJECT
    public:
        static constexpr const Column   defColumn   = Column::Key;
    
        FolderList(all_t, QWidget*parent=nullptr);
        FolderList(all_t, Column col, QWidget*parent=nullptr);
        ~FolderList();
    };
}
