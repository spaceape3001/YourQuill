////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/group/GroupProvider.hpp>
#include <yq/mithril/group/GroupCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_groups()
    {
        return [](){
            return ids(cdb::all_groups(Sorted::YES));
        };
    }
}

