////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/column.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class FolderModel;
    class FolderTable : public IdTableT<Folder> {
        Q_OBJECT
    public:

        static std::span<const Column>  defColumns();

        FolderTable(all_t, QWidget*parent=nullptr);
        FolderTable(all_t, std::initializer_list<Column> columns, QWidget*parent=nullptr);
        FolderTable(all_t, std::span<const Column> columns, QWidget*parent=nullptr);
        ~FolderTable();
        
        FolderModel*          model();
        const FolderModel*    model() const;
    };
}
