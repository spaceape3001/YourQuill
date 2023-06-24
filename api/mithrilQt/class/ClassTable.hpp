////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class ClassTable : public IdTableT<Class> {
        Q_OBJECT
    public:
        static std::span<const ColumnSpec>  defColumns();

        ClassTable(all_t, QWidget*parent=nullptr);
        ClassTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent=nullptr);
        ClassTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent=nullptr);
        ~ClassTable();
    };
}
