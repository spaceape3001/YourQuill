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
}
