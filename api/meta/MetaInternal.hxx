////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef YQ__CORE__OVERRIDE
    #error "Do NOT include this file.  It's internal to the YQ library ONLY!"
#endif

#include "MetaValueImpl.hxx"

namespace yqcore {
    enum {
        MV_Invalid      = 0,
        MV_Variant,
        MV_String,
        MV_Boolean,
        MV_Float,
        MV_Double,
        MV_Int8,
        MV_Int16,
        MV_Int32,
        MV_Int64,
        MV_UInt8,
        MV_UInt16,
        MV_UInt32,
        MV_UInt64,
        
        MV_USER
    };
    
};

using namespace yqcore;

        
#define MV_FIXED(type, number, ...)                                                         \
    MetaValueImpl<type>&     MetaValue::Binder<type>::editValue()                           \
    {                                                                                       \
        static MetaValueImpl<type>  *s_ret = new MetaValueImpl<type>(__FILE__,  number);    \
        return *s_ret;                                                                      \
    }                                                                                       \
    INTERNAL_MV_IMPLEMENT(type, __VA_ARGS__)
