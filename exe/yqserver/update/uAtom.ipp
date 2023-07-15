////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "uAtom.hpp"
#include <mithril/atom/AtomCDB.hpp>

namespace yq::mithril::update {
    UAtom&  UAtom::get(Atom x)
    {
        return lookup<UAtom, 65536>(x);
    }

    UAtom::UAtom(Atom x) : U<Atom>(x, cdb::key(x))
    {
    }
}

