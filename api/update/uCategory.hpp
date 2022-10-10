////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/db/CacheFwd.hpp>
#include <kernel/enum/Change.hpp>
#include <kernel/org/Category.hpp>

namespace yq {
    
    struct Document;
    struct Fragment;

    namespace update {

        //! \brief Erases the category from the cache
        void                    category_erase(Category);

        //! \brief Erases the category from the cache
        void                    category_erase(Document);

        //! Update the category icon
        void                    category_icon(Category);
        
        void                    category_notify(Fragment,Change);
        void                    category_notify_icons(Fragment,Change);

        void                    category_stage3(Document);

        //! General category update
        //! 
        //! Returns the merged data
        Category::SharedData    category_update(Category, cdb_options_t opts=0);
    }
}
