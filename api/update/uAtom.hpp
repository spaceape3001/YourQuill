////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"
#include <kernel/atom/Atom.hpp>

namespace yq {
    namespace update {
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
    }
}
