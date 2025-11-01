////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/group/GroupProvider.hpp>
#include <mithril/group/GroupCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_groups()
    {
        return [](){
            return ids(cdb::all_groups(Sorted::YES));
        };
    }
}

