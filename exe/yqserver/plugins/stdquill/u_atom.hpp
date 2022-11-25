////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/agw/Atom.hpp>
#include <kernel/agw/Attribute.hpp>
#include <kernel/notify/AtomNotifier.hpp>

namespace {

    /*! \brief Creates the atom from the attributes
    
        \note the creation will MODIFY/DESTROY the attributes.
    */
    yq::agw::Atom    s3_atom_create(KVTree& attrs, Document doc);
}


