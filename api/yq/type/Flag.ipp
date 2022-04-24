////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Flag.hpp"

namespace yq {
    std::string     flag_string(const EnumDef* def, uint64_t values, std::string_view sep)
    {
        std::string ret;
        if(!def || !values)
            return std::string();
        
        bool    f = true;
        for(auto& i : def->val2name()){
            if(i.first < 0)
                continue;
            if(i.first >= (int)(sizeof(values)*8))
                continue;
            if(!(values & (1ULL << i.first)))
                continue;
            if(f){
                f   = false;
            } else {
                ret += sep;
            }
            ret += i.second;
        }
        return ret;
    }
}
