////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uClass.hpp"
#include <mithril/class/ClassCDB.hpp>

namespace yq::mithril::update {
    UClass&  UClass::get(Class x)
    {
        return lookup<UClass,1024>(x);
    }

    UClass::UClass(Class x) : U<Class>(x, cdb::key(x)), doc(cdb::document(x))
    {
    }


}
