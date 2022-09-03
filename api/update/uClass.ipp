////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uClass.hpp"
#include <basic/Vector.hpp>
#include <kernel/db/SQ.hpp>

namespace yq {
    UClass&  uget(Class a)
    {
        static Vector<UClass*>   s_data;
        s_data.resize_if_under(a.id+1, 512, nullptr);
        UClass*& p  = s_data[a.id];
        if(!p)
            p       = new UClass(a);
        return *p;
    }
}

