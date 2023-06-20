////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DirectoryTable.hpp"
#include "DirectoryModel.hpp"


namespace yq::mithril {

    std::span<const Column>  DirectoryTable::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Hidden, Column::Path };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    DirectoryTable::DirectoryTable(all_t, QWidget*parent) : DirectoryTable(ALL, defColumns(), parent)
    {
    }
    
    DirectoryTable::DirectoryTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        DirectoryTable(ALL, span(columns), parent)
    {
    }
    
    DirectoryTable::DirectoryTable(all_t, std::span<const Column> columns, QWidget*parent) : 
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

