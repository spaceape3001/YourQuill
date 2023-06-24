////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FolderTable.hpp"

namespace yq::mithril {
    std::span<const ColumnSpec>  FolderTable::defColumns() 
    {
        static ColumnSpec   s_data[] = { { Column::Key, ColOpt::Icon}, Column::Name };
        return std::span<const ColumnSpec>(std::begin(s_data), std::end(s_data));
    }


    FolderTable::FolderTable(all_t, QWidget*parent) : FolderTable(ALL, defColumns(), parent)
    {
    }
    
    FolderTable::FolderTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent) : 
        FolderTable(ALL, span(columns), parent)
    {
    }
    
    FolderTable::FolderTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent) : 
        IdTableT<Folder>(ALL, parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Folders");
    }

    FolderTable::~FolderTable()
    {
    }
}

