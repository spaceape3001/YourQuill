////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class DirectoryTable : public IdTableT<Directory> {
        Q_OBJECT
    public:
        static std::span<const ColumnSpec>  defColumns();

        DirectoryTable(all_t, QWidget*parent=nullptr);
        DirectoryTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent=nullptr);
        DirectoryTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent=nullptr);
        ~DirectoryTable();
    };
}
