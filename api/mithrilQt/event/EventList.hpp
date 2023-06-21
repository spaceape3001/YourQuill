////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdList.hpp>

namespace yq::mithril {
    class EventModel;
    class EventList : public IdListT<Event> {
        Q_OBJECT
    public:
        static constexpr const Column   defColumn   = Column::Key;
    
        EventList(all_t, QWidget*parent=nullptr);
        EventList(all_t, Column col, QWidget*parent=nullptr);
        ~EventList();
        
        EventModel*          model();
        const EventModel*    model() const;
    };
}
