////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class FieldTable : public IdTableT<Field> {
        Q_OBJECT
    public:

        static std::span<const ColumnSpec>  defColumns();
    
        FieldTable(all_t, QWidget*parent=nullptr);
        FieldTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent=nullptr);
        FieldTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent=nullptr);
        ~FieldTable();
    };
}
