////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/enum/Change.hpp>

namespace yq {
    struct Field;
    struct Document;
    struct Fragment;
    
    namespace update {
        //! Erases the field from the cache
        void    field_erase(Field);
        void    field_erase(Document);

        void    field_icon(Field);
        
        void    field_notify(Fragment, Change);
        void    field_notify_icons(Fragment, Change);
        
        void    field_stage3(Document);
    }
}
