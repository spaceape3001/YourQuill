////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/FileSys.hpp>
#include <dbgui/Provider.hpp>

using DirectoryProvider = std::shared_ptr<Provider<Directory>>;

namespace provider {
    DirectoryProvider   all_directories();
}

