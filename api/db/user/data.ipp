////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "data.hpp"

namespace yq {
    User::Data&  User::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        bio += b.bio;
        permissions |= b.permissions;
        set_if_empty(brief, b.brief, fOverride);
        if(fOverride || !authentication)
            authentication  = b.authentication;
        return *this;
    }
    
    void  User::Data::reset()
    {
        *this   = Data();
    }
}
