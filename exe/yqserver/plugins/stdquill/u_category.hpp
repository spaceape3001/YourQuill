////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/CacheFwd.hpp>
#include <kernel/Change.hpp>
#include <kernel/Category.hpp>

namespace yq {
    
    struct Document;
    struct Fragment;

    //! \brief Erases the category from the cache
    void                    u_category_erase(Category);

    //! \brief Erases the category from the cache
    void                    u_category_erase(Document);

    //! Update the category icon
    void                    u_category_icon(Category);
    
    void                    u_category_notify(Fragment,Change);
    void                    u_category_notify_icons(Fragment,Change);

    void                    u_category_stage3(Document);

    //! General category update
    //! 
    //! Returns the merged data
    Category::SharedData    u_category_update(Category, cdb_options_t opts=0);
}
