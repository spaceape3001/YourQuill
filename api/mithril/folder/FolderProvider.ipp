////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/folder/FolderProvider.hpp>
#include <mithril/folder/FolderCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_folders()
    {
        return [](){
            return ids(cdb::all_folders(Sorted::YES));
        };
    }
}

