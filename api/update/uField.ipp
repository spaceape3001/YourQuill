////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uField.hpp"
#include <basic/Vector.hpp>
#include <kernel/db/SQ.hpp>

namespace yq {
    UField&  uget(Field a)
    {
        static Vector<UField*>   s_data;
        s_data.resize_if_under(a.id+1, 1024, nullptr);
        UField*& p  = s_data[a.id];
        if(!p)
            p       = new UField(a);
        return *p;
    }
}

