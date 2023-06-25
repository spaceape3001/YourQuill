////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>
#include <mithril/leaf/Leaf.hpp>

namespace yq::mithril {
    class LeafTable : public IdTableT<Leaf> {
        Q_OBJECT
    public:

        static std::span<const ColumnSpec>  defColumns();

        LeafTable(all_t, QWidget*parent=nullptr);
        LeafTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent=nullptr);
        LeafTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent=nullptr);
        ~LeafTable();
    };
}
