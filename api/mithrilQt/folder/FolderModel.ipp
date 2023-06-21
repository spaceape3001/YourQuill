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
    std::optional<IdColumn>     FolderModel::resolve(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            return column::folder_id(opts);
        case Column::Key:
            return column::folder_key(opts);
        case Column::Name:
            return column::folder_name(opts);
        default:
            return {};
        }
    }

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
    
    void    FolderModel::addColumn(Column col, ColOpts opts)
    {
        auto    cc  = resolve(col, opts);
        if(cc)
            addColumn(std::move(*cc));
    }

    void    FolderModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }

    void    FolderModel::setColumn(Column col, ColOpts opts)
    {
        auto    cc = resolve(col, opts);
        if(cc)
            setColumn(std::move(*cc));
    }
    
}
