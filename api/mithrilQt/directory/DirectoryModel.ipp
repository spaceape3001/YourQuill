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
    DirectoryModel::DirectoryModel(Type t, all_t, QObject* parent) : 
        DirectoryModel(t, Directory(), provider::all_directories(), parent)
    {
    }

    DirectoryModel::DirectoryModel(Type t, Directory rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Directory>(t, rt, std::move(prov), parent)
    {
    }
    
    DirectoryModel::~DirectoryModel()
    {
    }
    
    void    DirectoryModel::addColumn(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Hidden:
            addColumn(column::directory_hidden(opts));
            break;
        case Column::Id:
            addColumn(column::directory_id(opts));
            break;
        case Column::Key:
            addColumn(column::directory_key(opts));
            break;
        case Column::Name:
            addColumn(column::directory_name(opts));
            break;
        case Column::Path:
            addColumn(column::directory_path(opts));
            break;
        default:
            break;
        }
    }

    void    DirectoryModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }
    
}
