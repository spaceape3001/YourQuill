////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/column.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class EventModel;
    class EventTable : public IdTableT<Event> {
        Q_OBJECT
    public:
        EventTable(all_t, QWidget*parent=nullptr);
        EventTable(all_t, std::initializer_list<Column> columns, QWidget*parent=nullptr);
        EventTable(all_t, std::span<const Column> columns, QWidget*parent=nullptr);
        ~EventTable();
        
        EventModel*          model();
        const EventModel*    model() const;
    };
}
