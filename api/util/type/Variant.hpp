////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/meta/VariantDef.hpp>

namespace yq {



    boolean_r   to_boolean(const Variant&);
    double_r    to_double(const Variant&);
    float_r     to_float(const Variant&);
    int_r       to_int(const Variant&);
    int8_r      to_int8(const Variant&);
    int16_r     to_int16(const Variant&);
    int32_r     to_int32(const Variant&);
    int64_r     to_int64(const Variant&);
    integer_r   to_integer(const Variant&);
    short_r     to_short(const Variant&);
    string_r    to_string(const Variant&);
    uint8_r     to_uint8(const Variant&);
    uint16_r    to_uint16(const Variant&);
    uint32_r    to_uint32(const Variant&);
    uint64_r    to_uint64(const Variant&);
    unsigned_r  to_uinteger(const Variant&);
    ushort_r    to_ushort(const Variant&);
}

