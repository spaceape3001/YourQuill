////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/db/CacheFwd.hpp>
#include <kernel/enum/Change.hpp>
#include <kernel/user/User.hpp>

namespace yq {
    namespace update {
        //! Removes the user from the cache
        void                    user_erase(User);
        //! Removes the user from the cache
        void                    user_erase(Document);
        
        //! Updates user icon
        void                    user_icon(User);
        
        void                    user_notify(Fragment, Change);
        void                    user_notify_icons(Fragment, Change);
        
        void                    user_stage3(Document);
        
        //! Updates user data from files, return merged data
        User::SharedData        user_update(User, cdb_options_t opts=0);
    }
}
