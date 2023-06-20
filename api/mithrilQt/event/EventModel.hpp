////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/event/Event.hpp>
#include <mithrilQt/column.hpp>

namespace yq::mithril {
    class EventModel : public IdModelT<Event> {
        Q_OBJECT
    public:
    
        static std::span<const Column>  defColumns();
    
        EventModel(Type t) : EventModel(t, ALL) {}
        EventModel(Type t, all_t, std::span<const Column> cols=defColumns(), QObject* parent=nullptr);
        ~EventModel();
        
        using IdModelT<Event>::addColumn;
        void    addColumn(Column);
        
    private:
        EventModel(Type t, Event, IdProvider&&, std::span<const Column> cols, QObject*parent);
    };
}
