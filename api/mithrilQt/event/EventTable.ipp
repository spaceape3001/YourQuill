////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "EventTable.hpp"

namespace yq::mithril {
    std::span<const ColumnSpec>  EventTable::defColumns() 
    {
        static ColumnSpec   s_data[] = { { Column::Key, ColOpt::Icon}, Column::Title };
        return std::span<const ColumnSpec>(std::begin(s_data), std::end(s_data));
    }

    EventTable::EventTable(all_t, QWidget*parent) : EventTable(ALL, defColumns(), parent)
    {
    }
    
    EventTable::EventTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent) : 
        EventTable(ALL, span(columns), parent)
    {
    }
    
    EventTable::EventTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent) : 
        IdTableT<Event>(ALL, parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Events");
    }

    EventTable::~EventTable()
    {
    }
}

