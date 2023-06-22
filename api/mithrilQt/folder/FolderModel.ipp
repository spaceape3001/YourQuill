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
}
