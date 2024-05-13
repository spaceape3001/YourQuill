////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/user/UserProvider.hpp>
#include <mithril/user/UserCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_users()
    {
        return [](){
            return ids(cdb::all_users(Sorted::YES));
        };
    }
}

