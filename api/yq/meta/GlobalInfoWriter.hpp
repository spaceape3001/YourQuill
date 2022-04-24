////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Global.hpp"
#include "CompoundInfoStatic.hpp"

namespace yq {
    template <typename T>
    class GlobalInfo::Writer : public CompoundInfo::Static {
    public:
    
        Writer(GlobalInfo& p) : CompoundInfo::Static(&p) 
        {
        }
    };
}
