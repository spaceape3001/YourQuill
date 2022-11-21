////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/Atom.hpp>
#include <kernel/Attribute.hpp>
#include <kernel/AtomNotifier.hpp>

namespace {

    /*! \brief Creates the atom from the attributes
    
        \note the creation will MODIFY/DESTROY the attributes.
    */
    yq::Atom    s3_atom_create(KVTree& attrs, Document doc);
}


