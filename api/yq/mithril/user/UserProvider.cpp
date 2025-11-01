////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/user/UserProvider.hpp>
#include <yq/mithril/user/UserCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_users()
    {
        return [](){
            return ids(cdb::all_users(Sorted::YES));
        };
    }
}

