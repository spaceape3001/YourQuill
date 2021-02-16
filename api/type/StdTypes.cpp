#include "MetaInternal.hxx"

INVOKE( Variant(); );     // Ensure the invalid metatype is created.

namespace {
    String  print_string(const String&s) 
    {
        return s;
    }
    
    String  f_string(const String&v)
    {
        return v;
    }
    
    bool    p_string(String& v, const String& s)
    {
        v = s;
        return true;
    }
}

MV_FIXED(String, MV_String,
    print<print_string>();
    io_format<f_string>();
    io_parse<p_string>();
)

namespace {
    String  print_boolean(bool v)
    {
        return String::boolean(v);
    }
    
    String  f_boolean(bool v)
    {
        return v ? "true" : "false";
    }
    
    bool    p_boolean(bool& v, const String& s)
    {
        auto r = s.to_boolean();
        v   = r.value;
        return r.good;
    }
}

MV_FIXED(bool, MV_Boolean,
    label("Boolean");
    print<print_boolean>();
    io_format<f_boolean>();
    io_parse<p_boolean>();
)

namespace {
    String  print_double(double v)
    {
        return String::number(v);
    }
    
    String  f_double(double v)
    {
        return String::scientific(v, 19);
    }
    
    bool    p_double(double& v, const String& s)
    {
        auto r = s.to_double();
        v   = r.value;
        return r.good;
    }
}

MV_FIXED(double, MV_Double,
    label("Number");
    print<print_double>();
    io_format<f_double>();
    io_parse<p_double>();
    casts<float>();
)

namespace {
    String  print_float(float v)
    {
        return String::number(v);
    }
    
    String  f_float(float v)
    {
        return String::scientific(v, 7);
    }
    
    bool    p_float(float& v, const String& s)
    {
        auto r = s.to_float();
        v   = r.value;
        return r.good;
    }
}

MV_FIXED(float, MV_Float,
    print<print_float>();
    io_format<f_float>();
    io_parse<p_float>();
    casts<double>();
    tag("hide");
)

namespace {
    String  print_int8(int8_t v)
    {
        return String::number(v);
    }
    
    String  f_int8(int8_t v)
    {
        return String::number(v);
    }
    
    bool    p_int8(int8_t& v, const String& s)
    {
        auto r = s.to_int8();
        v       = r.value;
        return r.good;
    }
}


MV_FIXED(int8_t, MV_Int8,
    print<print_int8>();
    io_format<f_int8>();
    io_parse<p_int8>();
    casts<int16_t>();
    casts<int32_t>();
    casts<int64_t>();
    tag("hide");
)

namespace {
    String  print_int16(int16_t v)
    {
        return String::number(v);
    }
    
    String  f_int16(int16_t v)
    {
        return String::number(v);
    }
    
    bool    p_int16(int16_t& v, const String& s)
    {
        auto r = s.to_int16();
        v       = r.value;
        return r.good;
    }
}


MV_FIXED(int16_t, MV_Int16,
    print<print_int16>();
    io_format<f_int16>();
    io_parse<p_int16>();
    casts<int8_t>();
    casts<int16_t>();
    casts<int64_t>();
    tag("hide");
)


namespace {
    String  print_int32(int32_t v)
    {
        return String::number(v);
    }
    
    String  f_int32(int32_t v)
    {
        return String::number(v);
    }
    
    bool    p_int32(int32_t& v, const String& s)
    {
        auto r = s.to_int32();
        v       = r.value;
        return r.good;
    }
}


MV_FIXED(int32_t, MV_Int32,
    print<print_int32>();
    io_format<f_int32>();
    io_parse<p_int32>();
    casts<int8_t>();
    casts<int32_t>();
    casts<int64_t>();
    tag("hide");
)


namespace {
    String  print_int64(int64_t v)
    {
        return String::number(v);
    }
    
    String  f_int64(int64_t v)
    {
        return String::number(v);
    }
    
    bool    p_int64(int64_t& v, const String& s)
    {
        auto r = s.to_int64();
        v       = r.value;
        return r.good;
    }
}


MV_FIXED(int64_t, MV_Int64,
    label("Integer");
    print<print_int64>();
    io_format<f_int64>();
    io_parse<p_int64>();
    casts<int8_t>();
    casts<int16_t>();
    casts<int32_t>();
)


namespace {
    String  print_uint8(uint8_t v)
    {
        return String::number(v);
    }

    String  f_uint8(uint8_t v)
    {
        return String::number(v);
    }
    
    bool    p_uint8(uint8_t& v, const String& s)
    {
        auto r = s.to_uint8();
        v       = r.value;
        return r.good;
    }
}

MV_FIXED(uint8_t, MV_UInt8,
    print<print_uint8>();
    io_format<f_uint8>();
    io_parse<p_uint8>();
    tag("hide");
    casts<uint16_t>();
    casts<uint32_t>();
    casts<uint64_t>();
)


namespace {
    String  print_uint16(uint16_t v)
    {
        return String::number(v);
    }

    String  f_uint16(uint16_t v)
    {
        return String::number(v);
    }
    
    bool    p_uint16(uint16_t& v, const String& s)
    {
        auto r = s.to_uint16();
        v       = r.value;
        return r.good;
    }
}

MV_FIXED(uint16_t, MV_UInt16,
    print<print_uint16>();
    io_format<f_uint16>();
    io_parse<p_uint16>();
    tag("hide");
    casts<uint8_t>();
    casts<uint32_t>();
    casts<uint64_t>();
)


namespace {
    String  print_uint32(uint32_t v)
    {
        return String::number(v);
    }

    String  f_uint32(uint32_t v)
    {
        return String::number(v);
    }
    
    bool    p_uint32(uint32_t& v, const String& s)
    {
        auto r = s.to_uint32();
        v       = r.value;
        return r.good;
    }
}

MV_FIXED(uint32_t, MV_UInt32,
    print<print_uint32>();
    io_format<f_uint32>();
    io_parse<p_uint32>();
    tag("hide");
    casts<uint8_t>();
    casts<uint16_t>();
    casts<uint64_t>();
)


namespace {
    String  print_uint64(uint64_t v)
    {
        return String::number(v);
    }

    String  f_uint64(uint64_t v)
    {
        return String::number(v);
    }
    
    bool    p_uint64(uint64_t& v, const String& s)
    {
        auto r = s.to_uint64();
        v       = r.value;
        return r.good;
    }
}

MV_FIXED(uint64_t, MV_UInt64,
    label("Unsigned");
    print<print_uint64>();
    io_format<f_uint64>();
    io_parse<p_uint64>();
    casts<uint8_t>();
    casts<uint16_t>();
    casts<uint32_t>();
)



