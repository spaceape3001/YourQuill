#include "TypeInfo.hpp"
#include "Internal.hpp"
#include "Init.hpp"

#include <util/app/DelayInit.hpp>
#include <util/text/StringUtil.hpp>

MT_FIXED(MT_String, String)
MT_FIXED(MT_Boolean, bool)
MT_FIXED(MT_Float, float)
MT_FIXED(MT_Double, double)
MT_FIXED(MT_Int8, int8_t)
MT_FIXED(MT_Int16, int16_t)
MT_FIXED(MT_Int32, int32_t)
MT_FIXED(MT_Int64, int64_t)
MT_FIXED(MT_UInt8, uint8_t)
MT_FIXED(MT_UInt16, uint16_t)
MT_FIXED(MT_UInt32, uint32_t)
MT_FIXED(MT_UInt64, uint64_t)

namespace yq {
    namespace {
    
        bool    parse_string(const std::string_view& txt, String& v)
        {
            v   = txt;
            return true;
        }
    
        bool    parse_boolean(const std::string_view&txt, bool&v)
        {
            auto r  = to_boolean(txt);
            v   = r.value;
            return r.good;
        }
        
        bool    parse_double(const std::string_view&txt, double&v)
        {
            auto r  = to_double(txt);
            v   = r.value;
            return r.good;
        }
        
        bool    parse_float(const std::string_view&txt, float&v)
        {
            auto r  = to_float(txt);
            v   = r.value;
            return r.good;
        }
        
        
        bool    parse_int8(const std::string_view&txt, int8_t&v)
        {
            auto r  = to_int8(txt);
            v   = r.value;
            return r.good;
        }
        
        bool    parse_int16(const std::string_view&txt, int16_t&v)
        {
            auto r  = to_int16(txt);
            v   = r.value;
            return r.good;
        }
        
        bool    parse_int32(const std::string_view&txt, int32_t&v)
        {
            auto r  = to_int32(txt);
            v   = r.value;
            return r.good;
        }
        
        bool    parse_int64(const std::string_view&txt, int64_t&v)
        {
            auto r  = to_int64(txt);
            v   = r.value;
            return r.good;
        }

        bool    parse_uint8(const std::string_view&txt, uint8_t&v)
        {
            auto r  = to_uint8(txt);
            v   = r.value;
            return r.good;
        }
        
        bool    parse_uint16(const std::string_view&txt, uint16_t&v)
        {
            auto r  = to_uint16(txt);
            v   = r.value;
            return r.good;
        }
        
        bool    parse_uint32(const std::string_view&txt, uint32_t&v)
        {
            auto r  = to_uint32(txt);
            v   = r.value;
            return r.good;
        }
        
        bool    parse_uint64(const std::string_view&txt, uint64_t&v)
        {
            auto r  = to_uint64(txt);
            v   = r.value;
            return r.good;
        }
    
        void    init_stdtypes()
        {
            meta_write<bool>().parse<parse_boolean>();
            meta_write<double>().parse<parse_double>();
            meta_write<float>().parse<parse_float>();
            meta_write<int8_t>().parse<parse_int8>();
            meta_write<int16_t>().parse<parse_int16>();
            meta_write<int32_t>().parse<parse_int32>();
            meta_write<int64_t>().parse<parse_int64>();
            meta_write<uint8_t>().parse<parse_uint8>();
            meta_write<uint16_t>().parse<parse_uint16>();
            meta_write<uint32_t>().parse<parse_uint32>();
            meta_write<uint64_t>().parse<parse_uint64>();
            meta_write<String>().parse<parse_string>();
        }
    }
}

YQ_INVOKE(
    yq::init_stdtypes();
)
