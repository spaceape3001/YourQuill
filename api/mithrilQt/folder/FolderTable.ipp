////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FolderTable.hpp"
#include "FolderModel.hpp"


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
        IdTableT<Folder>(new FolderModel(IdModel::Type::Table, ALL), parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Folders");
    }

    FolderTable::~FolderTable()
    {
    }
    
    
    FolderModel*          FolderTable::model()
    {
        return static_cast<FolderModel*>(m_model);
    }
    
    const FolderModel*    FolderTable::model() const
    {
        return static_cast<const FolderModel*>(m_model);
    }
}

