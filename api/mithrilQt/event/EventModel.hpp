////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdModel.hpp>
#include <mithril/event/Event.hpp>
#include <mithrilQt/preamble.hpp>

namespace yq::mithril {
    class EventModel : public IdModelT<Event> {
        Q_OBJECT
    public:

        EventModel(Type t, QObject* parent=nullptr) : EventModel(t, ALL, parent) {}
        EventModel(Type t, all_t, QObject* parent=nullptr);
        ~EventModel();
        
    private:
        EventModel(Type t, Event, IdProvider&&, QObject*parent);
    };
}
