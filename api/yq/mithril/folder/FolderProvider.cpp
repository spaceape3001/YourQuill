////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/folder/FolderProvider.hpp>
#include <yq/mithril/folder/FolderCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_folders()
    {
        return [](){
            return ids(cdb::all_folders(Sorted::YES));
        };
    }
}

