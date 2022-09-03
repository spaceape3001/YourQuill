////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"
#include <kernel/org/Category.hpp>

namespace yq {

    //! Update cache data for category (from files)
    Category::SharedData    update(Category, cdb_options_t opts=0);
    
    //! Update category's icon
    void                    update_icon(Category);

}
