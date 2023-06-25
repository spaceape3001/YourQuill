////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>
#include <mithril/event/Event.hpp>

namespace yq::mithril {
    class EventTable : public IdTableT<Event> {
        Q_OBJECT
    public:
        static std::span<const ColumnSpec>  defColumns();
    
        EventTable(all_t, QWidget*parent=nullptr);
        EventTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent=nullptr);
        EventTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent=nullptr);
        ~EventTable();
    };
}
