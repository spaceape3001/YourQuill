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
}

namespace {
    //! Erases the leaf
    void    u_leaf_erase(Leaf);
    //void    u_leaf_erase(Document);
    
    //! Updates the leaf icon
    void    u_leaf_icon(Leaf);

    void    u_leaf_notify_icons(Fragment, Change);
    
    void    u_leaf_stage3_pass1(Document);
    void    u_leaf_stage3_pass2(Document);
    
    void    u_leaf_stage4_cleanup();
}
