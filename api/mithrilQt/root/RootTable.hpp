////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>
#include <mithril/root/Root.hpp>

namespace yq::mithril {
    class RootTable : public IdTableT<Root> {
        Q_OBJECT
    public:
        static std::span<const ColumnSpec>  defColumns();

        RootTable(all_t, QWidget*parent=nullptr);
        RootTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent=nullptr);
        RootTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent=nullptr);
        ~RootTable();
    };
}
