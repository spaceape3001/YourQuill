////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DirectoryModel.hpp"
#include "DirectoryColumn.hpp"
#include <mithril/directory/DirectoryProvider.hpp>

namespace yq::mithril {
    std::span<const Column>  DirectoryModel::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Hidden, Column::Path };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    DirectoryModel::DirectoryModel(Type t, all_t, std::span<const Column> cols, QObject* parent) : 
        DirectoryModel(t, Directory(), provider::all_directories(), cols, parent)
    {
    }

    DirectoryModel::DirectoryModel(Type t, Directory rt, IdProvider&& prov, std::span<const Column> cols, QObject*parent) : 
        IdModelT<Directory>(t, rt, std::move(prov), parent)
    {
        for(Column c : cols)
            addColumn(c);
        reload();
    }
    
    DirectoryModel::~DirectoryModel()
    {
    }
    
    void    DirectoryModel::addColumn(Column col)
    {
        switch(col){
        case Column::Hidden:
            addColumn(column::directory_hidden());
            break;
        case Column::Id:
            addColumn(column::directory_id());
            break;
        case Column::Key:
            addColumn(column::directory_key());
            break;
        case Column::Name:
            addColumn(column::directory_name());
            break;
        case Column::Path:
            addColumn(column::directory_path());
            break;
        default:
            break;
        }
    }
    
}
