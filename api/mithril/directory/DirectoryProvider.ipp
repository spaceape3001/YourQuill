////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/directory/DirectoryProvider.hpp>
#include <mithril/directory/DirectoryCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_directories()
    {
        return [](){
            return ids(cdb::all_directories(Sorted::YES));
        };
    }
}

