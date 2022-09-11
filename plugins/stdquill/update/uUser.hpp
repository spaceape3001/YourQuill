////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"
#include <kernel/user/User.hpp>

namespace yq {
    //! Updates user data from files, return merged data
    User::SharedData        update(User, cdb_options_t opts=0);
    
    //! Updates user icon
    void                    update_icon(User);
}
