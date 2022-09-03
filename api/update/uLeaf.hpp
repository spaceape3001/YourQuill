////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"
#include <kernel/leaf/Leaf.hpp>
#include <kernel/file/Document.hpp>

namespace yq {
    struct ULeaf {
        union {
            const uint64_t      id;
            const Leaf          leaf;
            const Document      doc;
        };
        bool                deleted = false;
        ULeaf(Leaf a) : leaf(a) {}
    };

    //! Note, not thread-safe, call from ONE thread only!
    ULeaf&  uget(Leaf);


    /*! Updates the leaf data
    
        \note This updates ONLY the leaf data, no associations are
        covered.
        
        \return The merged data.
    */
    Leaf::SharedData            update(Leaf, cdb_options_t opts=0);
    
    //! Updates the leaf icon
    void                        update_icon(Leaf);

}
