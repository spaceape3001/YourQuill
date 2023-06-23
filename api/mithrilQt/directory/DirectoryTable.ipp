////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DirectoryTable.hpp"
#include "DirectoryModel.hpp"


namespace yq::mithril {

    std::span<const ColumnSpec>  DirectoryTable::defColumns() 
    {
        static ColumnSpec   s_data[] = { { Column::Key, ColOpt::Icon}, Column::Hidden, Column::Path };
        return std::span<const ColumnSpec>(std::begin(s_data), std::end(s_data));
    }

    DirectoryTable::DirectoryTable(all_t, QWidget*parent) : DirectoryTable(ALL, defColumns(), parent)
    {
    }
    
    DirectoryTable::DirectoryTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent) : 
        DirectoryTable(ALL, span(columns), parent)
    {
    }
    
    DirectoryTable::DirectoryTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent) : 
        IdTableT<Directory>(new DirectoryModel(IdModel::Type::Table, ALL), parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Directories");
    }

    DirectoryTable::~DirectoryTable()
    {
    }
    
    
    DirectoryModel*          DirectoryTable::model()
    {
        return static_cast<DirectoryModel*>(m_model);
    }
    
    const DirectoryModel*    DirectoryTable::model() const
    {
        return static_cast<const DirectoryModel*>(m_model);
    }
}

