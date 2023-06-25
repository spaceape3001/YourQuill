////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>
#include <mithril/place/Place.hpp>

namespace yq::mithril {
    class PlaceTable : public IdTableT<Place> {
        Q_OBJECT
    public:
        static std::span<const ColumnSpec>  defColumns();
    
        PlaceTable(all_t, QWidget*parent=nullptr);
        PlaceTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent=nullptr);
        PlaceTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent=nullptr);
        ~PlaceTable();
    };
}
