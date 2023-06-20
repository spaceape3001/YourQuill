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
    std::span<const Column>  FolderModel::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Name };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    FolderModel::FolderModel(Type t, all_t, std::span<const Column> cols, QObject* parent) : 
        FolderModel(t, Folder(), provider::all_folders(), cols, parent)
    {
    }

    FolderModel::FolderModel(Type t, Folder rt, IdProvider&& prov, std::span<const Column> cols, QObject*parent) : 
        IdModelT<Folder>(t, rt, std::move(prov), parent)
    {
        for(Column c : cols)
            addColumn(c);
        reload();
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
    
}
