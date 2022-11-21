////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/CacheFwd.hpp>
#include <kernel/Change.hpp>
#include <kernel/User.hpp>

namespace yq {
    //! Removes the user from the cache
    void                    u_user_erase(User);
    //! Removes the user from the cache
    void                    u_user_erase(Document);
    
    //! Updates user icon
    void                    u_user_icon(User);
    
    void                    u_user_notify(Fragment, Change);
    void                    u_user_notify_icons(Fragment, Change);
    
    void                    u_user_stage3(Document);
    
    //! Updates user data from files, return merged data
    User::SharedData        u_user_update(User, cdb_options_t opts=0);
}
