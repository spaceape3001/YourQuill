////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DirectoryTable.hpp"
#include "DirectoryModel.hpp"


namespace yq::mithril {

    DirectoryTable::DirectoryTable(all_t, QWidget*parent) : DirectoryTable(ALL, DirectoryModel::defColumns(), parent)
    {
    }
    
    DirectoryTable::DirectoryTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        DirectoryTable(ALL, span(columns), parent)
    {
    }
    
    DirectoryTable::DirectoryTable(all_t, std::span<const Column> columns, QWidget*parent) : 
        IdTableT<Directory>(new DirectoryModel(IdModel::Type::Table, ALL, columns), parent)
    {
        setWindowTitle("Directorys");
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

