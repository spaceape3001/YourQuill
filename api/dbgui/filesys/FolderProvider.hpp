////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/FileSys.hpp>
#include <gui/Provider.hpp>

using FolderProvider = std::shared_ptr<Provider<Folder>>;

namespace provider {
    FolderProvider      all_folders();
}

