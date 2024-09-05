////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UserData.hpp"

#include <yq-toolbox/container/vector_utils.hpp>
//#include <0/basic/CollectionUtils.hpp>


namespace yq::mithril {
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
