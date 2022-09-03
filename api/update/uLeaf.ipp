////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uLeaf.hpp"
#include <basic/Vector.hpp>
#include <kernel/db/SQ.hpp>

namespace yq {
    ULeaf&  uget(Leaf a)
    {
        static Vector<ULeaf*>   s_data;
        s_data.resize_if_under(a.id+1, 4096, nullptr);
        ULeaf*& p   = s_data[a.id];
        if(!p)
            p       = new ULeaf(a);
        return *p;
    }
}

