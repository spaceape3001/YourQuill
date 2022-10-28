////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"
#include <kernel/atom/Atom.hpp>
#include <kernel/attr/Attribute.hpp>

namespace yq {

    struct UAtom {
        union {
            const uint64_t    id;
            const Atom        atom;
        };
        bool                deleted = false;
        UAtom(Atom a) : atom(a) {}
    };

    //! Note, not thread-safe, call from ONE thread only!
    UAtom&  uget(Atom);
    
    void    update_atom(Atom, Document, const Attribute::Report&, cdb_options_t = 0, const ClassSet& cset=ClassSet());
}
