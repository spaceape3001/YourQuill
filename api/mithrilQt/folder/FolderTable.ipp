////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FolderTable.hpp"
#include "FolderModel.hpp"


namespace yq::mithril {

    FolderTable::FolderTable(all_t, QWidget*parent) : FolderTable(ALL, FolderModel::defColumns(), parent)
    {
    }
    
    FolderTable::FolderTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        FolderTable(ALL, span(columns), parent)
    {
    }
    
    FolderTable::FolderTable(all_t, std::span<const Column> columns, QWidget*parent) : 
        IdTableT<Folder>(new FolderModel(IdModel::Type::Table, ALL, columns), parent)
    {
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

