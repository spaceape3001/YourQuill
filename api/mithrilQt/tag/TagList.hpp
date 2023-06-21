////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdList.hpp>

namespace yq::mithril {
    class TagModel;
    class TagList : public IdListT<Tag> {
        Q_OBJECT
    public:
        static constexpr const Column   defColumn   = Column::Name;
    
        TagList(all_t, QWidget*parent=nullptr);
        TagList(all_t, Column col, QWidget*parent=nullptr);
        ~TagList();
        
        TagModel*          model();
        const TagModel*    model() const;
    };
}
