////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uClass.hpp"
#include <mithril/class/ClassCDB.hpp>

namespace yq::mithril::update {
    UClass&  UClass::get(Class c)
    {
        static constexpr const size_t       kMinAlloc   = 1024uz;
        static std::vector<UClass*>      s_lookup(kMinAlloc, nullptr);
        if(s_lookup.size() <= c.id)
            s_lookup.resize(c.id+kMinAlloc, nullptr);
        
        auto& u = s_lookup[c.id];
        if(!u)
            u   = new UClass(c);
        return *u;
    }

    UClass::UClass(Class c) : class_(c), id(c.id), key(cdb::key(c)) 
    {
    }


}
