////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class TagTable : public IdTableT<Tag> {
        Q_OBJECT
    public:
        static std::span<const ColumnSpec>  defColumns();
    
        TagTable(all_t, QWidget*parent=nullptr);
        TagTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent=nullptr);
        TagTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent=nullptr);
        ~TagTable();
    };
}
