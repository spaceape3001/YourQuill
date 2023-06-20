////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FolderModel.hpp"
#include "FolderColumn.hpp"
#include <mithril/folder/FolderProvider.hpp>

namespace yq::mithril {
    FolderModel::FolderModel(Type t, all_t, QObject* parent) : 
        FolderModel(t, Folder(), provider::all_folders(), parent)
    {
    }

    FolderModel::FolderModel(Type t, Folder rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Folder>(t, rt, std::move(prov), parent)
    {
    }
    
    FolderModel::~FolderModel()
    {
    }
    
    void    FolderModel::addColumn(Column col)
    {
        switch(col){
        case Column::Id:
            addColumn(column::folder_id());
            break;
        case Column::Key:
            addColumn(column::folder_key());
            break;
        case Column::Name:
            addColumn(column::folder_name());
            break;
        default:
            break;
        }
    }

    void    FolderModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }
    
}
