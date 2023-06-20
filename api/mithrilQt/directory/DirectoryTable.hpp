////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/column.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class DirectoryModel;
    class DirectoryTable : public IdTableT<Directory> {
        Q_OBJECT
    public:
        static std::span<const Column>  defColumns();

        DirectoryTable(all_t, QWidget*parent=nullptr);
        DirectoryTable(all_t, std::initializer_list<Column> columns, QWidget*parent=nullptr);
        DirectoryTable(all_t, std::span<const Column> columns, QWidget*parent=nullptr);
        ~DirectoryTable();
        
        DirectoryModel*          model();
        const DirectoryModel*    model() const;
    };
}
