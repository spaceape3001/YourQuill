////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "stdint.h"

struct UInt128 { 
    uint64_t low, high; 
    UInt128() : low{}, high{} {}
    UInt128(uint64_t l, uint64_t h) : low(l), high(h) {}
    operator unsigned __int128 () const { return *(const unsigned __int128*) this; }
};
using uint128_t = unsigned __int128;
using int128_t  = __int128;
