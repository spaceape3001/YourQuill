////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/atom/Atom.hpp>
#include <kernel/attr/Attribute.hpp>
#include <kernel/notify/AtomNotifier.hpp>

namespace {

    /*! \brief Creates the atom from the attributes
    
        \note the creation will MODIFY/DESTROY the attributes.
    */
    Atom    s3_atom_create(KVTree& attrs, Document doc);
}


