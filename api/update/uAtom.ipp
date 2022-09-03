////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uAtom.hpp"
#include <basic/Vector.hpp>

namespace yq {
    namespace update {
        UAtom&  uget(Atom a)
        {
            static Vector<UAtom*>   s_data;
            s_data.resize_if_under(a.id+1, 65536, nullptr);
            UAtom*& p   = s_data[a.id];
            if(!p)
                p       = new UAtom(a);
            return *p;
        }
    }
}

