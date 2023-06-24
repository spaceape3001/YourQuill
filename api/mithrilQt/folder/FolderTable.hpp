////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class FolderTable : public IdTableT<Folder> {
        Q_OBJECT
    public:

        static std::span<const ColumnSpec>  defColumns();

        FolderTable(all_t, QWidget*parent=nullptr);
        FolderTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent=nullptr);
        FolderTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent=nullptr);
        ~FolderTable();
    };
}
