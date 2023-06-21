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
    std::optional<IdColumn>     DirectoryModel::resolve(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Hidden:
            return column::directory_hidden(opts);
        case Column::Id:
            return column::directory_id(opts);
        case Column::Key:
            return column::directory_key(opts);
        case Column::Name:
            return column::directory_name(opts);
        case Column::Path:
            return column::directory_path(opts);
        default:
            return {};
        }
    }

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
        auto    cc  = resolve(col, opts);
        if(cc)
            addColumn(std::move(*cc));
    }

    void    DirectoryModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }

    void    DirectoryModel::setColumn(Column col, ColOpts opts)
    {
        auto    cc = resolve(col, opts);
        if(cc)
            setColumn(std::move(*cc));
    }
    
}
