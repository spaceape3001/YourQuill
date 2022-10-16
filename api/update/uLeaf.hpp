////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/enum/Change.hpp>

namespace yq {
    struct Leaf;
    struct Document;
    struct Fragment;

    namespace update {
        //! Erases the leaf
        void    leaf_erase(Leaf);
        void    leaf_erase(Document);
        
        //! Updates the leaf icon
        void    leaf_icon(Leaf);

        void    leaf_notify_icons(Fragment, Change);
        
        void    leaf_stage3_pass1_declare(Document);
    }
}
