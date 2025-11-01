////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/directory/DirectoryProvider.hpp>
#include <yq/mithril/directory/DirectoryCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_directories()
    {
        return [](){
            return ids(cdb::all_directories(Sorted::YES));
        };
    }
}

