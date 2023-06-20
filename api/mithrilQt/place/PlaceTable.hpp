////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/column.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class PlaceModel;
    class PlaceTable : public IdTableT<Place> {
        Q_OBJECT
    public:
        PlaceTable(all_t, QWidget*parent=nullptr);
        PlaceTable(all_t, std::initializer_list<Column> columns, QWidget*parent=nullptr);
        PlaceTable(all_t, std::span<const Column> columns, QWidget*parent=nullptr);
        ~PlaceTable();
        
        PlaceModel*          model();
        const PlaceModel*    model() const;
    };
}
