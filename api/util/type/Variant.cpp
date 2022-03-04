#include "Variant.hpp"

#include <util/meta/Internal.hpp>
#include <util/stream/Text.hpp>
#include <util/text/Utils.hpp>


namespace yq {
    bool    Variant::good(const TypeInfo&ti)
    {
        //  these are the function pointers that won't be checked at runtime
        return ti.m_copyB && ti.m_copyR && ti.m_ctorCopyR && ti.m_ctorCopyB 
            && ti.m_ctorMove && ti.m_dtor && ti.m_equal && ti.m_moveB;
    }


    Variant      Variant::parse_me(const TypeInfo& ti, const std::string_view&txt)
    {
        Variant ret;
        bool    f   = ret.parse(ti, txt);
        return f ? ret : Variant();
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    Variant::Variant(const Variant& cp) : m_type(cp.m_type)
    {
        (m_type -> m_ctorCopyB)(m_data, cp.m_data);
    }
    
    Variant::Variant(Variant&&mv) : m_type(mv.m_type)
    {
        (m_type -> m_ctorMove)(m_data, std::move(mv.m_data));
        mv.m_type   = &invalid();
    }
    
    Variant::Variant(const TypeInfo* newTypePtr) 
    {
        assert(newTypePtr && good(*newTypePtr));
        m_type  = newTypePtr;
        (m_type -> m_ctorCopyB)(m_data, m_type -> m_default);
    }
    
    //! Creates a defaulted Variant to the specified meta-type
    Variant::Variant(const TypeInfo&newType) : m_type(&newType)
    {
        assert(good(newType));
        (m_type -> m_ctorCopyB)(m_data, m_type -> m_default);
    }
    
    Variant::Variant(const TypeInfo&newType, const void* dp) : m_type(&newType)
    {
        assert(good(newType));

        if(dp)
            (m_type -> m_ctorCopyR)(m_data, dp);
        else 
            (m_type -> m_ctorCopyB)(m_data, m_type -> m_default);
    }
    

    Variant::Variant(char ch)
    {
        set<std::string>(to_string(ch));
    }
    
    Variant::Variant(char8_t ch)
    {
        set<std::string>(to_string(ch));
    }
    
    Variant::Variant(char32_t ch)
    {
        set<std::string>(copy(to_string(ch)));
    }
    
    Variant::Variant(const char*z)
    {
        set<std::string>(z);
    }
    
    Variant::Variant(char*z)
    {
        set<std::string>(z);
    }

    Variant::Variant(const char8_t*z)
    {
        set<std::string>(copy(to_string(z)));
    }
    
    Variant::Variant(const char32_t*z)
    {
        set<std::string>(to_string(z));
    }
    
    Variant::Variant(const std::u8string&z)
    {
        set<std::string>(copy(to_string(z)));
    }
    
    Variant::Variant(const std::u32string&z)
    {
        set<std::string>(to_string(z));
    }
    
    Variant::Variant(const std::wstring&z)
    {
        set<std::string>(to_string(z));
    }
    

    //  Variant(const meta::Type*);

    Variant::~Variant()
    {
        assert(m_type);
        (m_type -> m_dtor)(m_data);
        
        #ifndef NDEBUG
        m_type      = &invalid();
        #endif
    }
    

    Variant&        Variant::operator=(const Variant& cp)
    {
        assert(m_type);
        if(&cp != this){
            if(cp.m_type == m_type){
                assert(m_type -> m_copyB);

                (m_type -> m_copyB)(m_data, cp.m_data);
            } else {
                assert(m_type -> m_dtor);
                assert(m_type -> m_ctorCopyB);

                (m_type -> m_dtor)(m_data);
                m_type  = cp.m_type;
                (m_type -> m_ctorCopyB)(m_data, cp.m_data);
            }
        }
        return *this;
    }
    
    Variant&        Variant::operator=(Variant&&mv)
    {
        assert(m_type);
        if(&mv != this){
            if(mv.m_type == m_type){
                (m_type -> m_moveB)(m_data, std::move(mv.m_data));
            } else {
                (m_type -> m_dtor)(m_data);
                m_type  = mv.m_type;
                (m_type -> m_ctorMove)(m_data, std::move(mv.m_data));
            }
            mv.m_type   = &invalid();
        }
        return *this;
    }
    
    bool            Variant::operator==(const Variant&b) const
    {
        if(this == &b)
            return true;
        if(m_type != b.m_type)
            return false;
        return (m_type->m_equal)(m_data, b.m_data);
    }
    

    bool            Variant::can_convert(const TypeInfo&newType) const
    {
        if(m_type == &newType)
            return true;
        if(&newType == &invalid())
            return true;
        if(&newType == &variant())
            return true;
        return m_type->m_convert.has(&newType);
    }

    void            Variant::clear()
    {
        if(m_type){
            m_type -> m_dtor(m_data);
            m_type  = &invalid();
        }
    }
    

    Variant         Variant::convert(const TypeInfo& newType) const
    {
        assert(m_type);

        if(&newType == m_type)
            return *this;
        if(&newType == &invalid())
            return Variant();
        if(m_type == &invalid())
            return Variant();
        if(&newType == &variant())
            return *this;
            
        auto fn = m_type -> m_convert.get(&newType, nullptr);
        if(!fn)
            return Variant();
        
        Variant ret(newType);
        fn(ret.raw_ptr(), raw_ptr());
        return ret;
    }


    bool            Variant::is_valid() const
    {
        return m_type && (m_type != &invalid());
    }

    bool            Variant::parse(const TypeInfo&newType, const std::string_view&txt)
    {   
        assert(m_type);
        if(&newType != m_type){
            assert(good(newType));
            assert(m_type->m_dtor);
            (m_type->m_dtor)(m_data);
            m_type  = &newType;
        }
        
        (m_type->m_ctorCopyB)(m_data, m_type->m_default);
        return (m_type->m_parse)(raw_ptr(), txt);
    }
    

    bool            Variant::print(Stream& str) const
    {
        assert(m_type);
        if(m_type->m_print){
            (m_type->m_print)(str, raw_ptr());
            return true;
        }
        return false;
    }

    std::string          Variant::printable() const
    {
        std::string  result;
        stream::Text  buf(result);
        print(buf);
        buf.flush();
        return result;  
    }

    const void*      Variant::raw_ptr() const 
    {
        return m_type -> is_small() ? (const void*) m_data.Data : m_data.Pointer;
    };
    
    void*            Variant::raw_ptr() 
    {
        return m_type -> is_small() ? (void*) m_data.Data : m_data.Pointer;
    };

    void    Variant::set(const TypeInfo&newType, const void*val)
    {
        if(m_type){
            if(&newType == m_type){
                assert(m_type -> m_copyR);
                m_type -> m_copyR(m_data, val);
                return ;
            }
            m_type -> m_dtor(m_data);
        }

        assert(good(newType));
        m_type  = &newType;
        if(val){
            m_type -> m_ctorCopyR(m_data, val);
        } else
            m_type -> m_ctorCopyB(m_data, m_type->m_default);
    }

    bool        Variant::write(Stream&str) const
    {
        assert(m_type);
        if(m_type -> m_write){
            (m_type->m_write)(str, raw_ptr());
            return true;
        }
        return false;
    }
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    boolean_r   to_boolean(const Variant&v)
    {
        switch(v.type().id()){
        case MT_String:
            return to_boolean(v.data().reference<std::string>());
        case MT_Boolean:
            return boolean_r{ v.data().reference<bool>(), true };
        case MT_Int8:
            return boolean_r{ static_cast<bool>( v.data().reference<int8_t>()), true };
        case MT_Int16:
            return boolean_r{ static_cast<bool>( v.data().reference<int16_t>()), true };
        case MT_Int32:
            return boolean_r{ static_cast<bool>( v.data().reference<int32_t>()), true };
        case MT_Int64:
            return boolean_r{ static_cast<bool>( v.data().reference<int64_t>()), true };
        case MT_UInt8:
            return boolean_r{ static_cast<bool>( v.data().reference<uint8_t>()), true };
        case MT_UInt16:
            return boolean_r{ static_cast<bool>( v.data().reference<uint16_t>()), true };
        case MT_UInt32:
            return boolean_r{ static_cast<bool>( v.data().reference<uint32_t>()), true };
        case MT_UInt64:
            return boolean_r{ static_cast<bool>( v.data().reference<uint64_t>()), true };
        default:
            return {};
        }
    }
    
    double_r    to_double(const Variant&v)
    {
        switch(v.type().id()){
        case MT_String:
            return to_double(v.data().reference<std::string>());
        case MT_Boolean:
            return double_r{ (v.data().reference<bool>()?1.0:0.0), true };
        case MT_Double:
            return double_r{ v.data().reference<double>(), true };
        case MT_Float:
            return double_r{ v.data().reference<float>(), true };
        case MT_Int8:
            return double_r{ static_cast<double>( v.data().reference<int8_t>()), true };
        case MT_Int16:
            return double_r{ static_cast<double>( v.data().reference<int16_t>()), true };
        case MT_Int32:
            return double_r{ static_cast<double>( v.data().reference<int32_t>()), true };
        case MT_Int64:
            return double_r{ static_cast<double>( v.data().reference<int64_t>()), true };
        case MT_UInt8:
            return double_r{ static_cast<double>( v.data().reference<uint8_t>()), true };
        case MT_UInt16:
            return double_r{ static_cast<double>( v.data().reference<uint16_t>()), true };
        case MT_UInt32:
            return double_r{ static_cast<double>( v.data().reference<uint32_t>()), true };
        case MT_UInt64:
            return double_r{ static_cast<double>( v.data().reference<uint64_t>()), true };
        default:
            return {};
        }
    }
    
    float_r     to_float(const Variant&v)
    {
        switch(v.type().id()){
        case MT_String:
            return to_float(v.data().reference<std::string>());
        case MT_Boolean:
            return float_r{ v.data().reference<bool>()?1.0f:0.0f, true };
        case MT_Double:
            return float_r{ (float) v.data().reference<float>(), true };
        case MT_Float:
            return float_r{ v.data().reference<float>(), true };
        case MT_Int8:
            return float_r{ static_cast<float>( v.data().reference<int8_t>()), true };
        case MT_Int16:
            return float_r{ static_cast<float>( v.data().reference<int16_t>()), true };
        case MT_Int32:
            return float_r{ static_cast<float>( v.data().reference<int32_t>()), true };
        case MT_Int64:
            return float_r{ static_cast<float>( v.data().reference<int64_t>()), true };
        case MT_UInt8:
            return float_r{ static_cast<float>( v.data().reference<uint8_t>()), true };
        case MT_UInt16:
            return float_r{ static_cast<float>( v.data().reference<uint16_t>()), true };
        case MT_UInt32:
            return float_r{ static_cast<float>( v.data().reference<uint32_t>()), true };
        case MT_UInt64:
            return float_r{ static_cast<float>( v.data().reference<uint64_t>()), true };
        default:
            return {};
        }
    }
    
    int_r       to_int(const Variant&v)
    {
        switch(v.type().id()){
        case MT_String:
            return to_int(v.data().reference<std::string>());
        case MT_Boolean:
            return int_r{ static_cast<int>(v.data().reference<bool>()), true };
        case MT_Float:
            return int_r{ static_cast<int>(v.data().reference<float>()), true };
        case MT_Double:
            return int_r{ static_cast<int>(v.data().reference<double>()), true };
        case MT_Int8:
            return int_r{ static_cast<int>( v.data().reference<int8_t>()), true };
        case MT_Int16:
            return int_r{ static_cast<int>( v.data().reference<int16_t>()), true };
        case MT_Int32:
            return int_r{ static_cast<int>( v.data().reference<int32_t>()), true };
        case MT_Int64:
            return int_r{ static_cast<int>( v.data().reference<int64_t>()), true };
        case MT_UInt8:
            return int_r{ static_cast<int>( v.data().reference<uint8_t>()), true };
        case MT_UInt16:
            return int_r{ static_cast<int>( v.data().reference<uint16_t>()), true };
        case MT_UInt32:
            return int_r{ static_cast<int>( v.data().reference<uint32_t>()), true };
        case MT_UInt64:
            return int_r{ static_cast<int>( v.data().reference<uint64_t>()), true };
        default:
            return {};
        }
    }
    
    #if 0
    int8_r      to_int8(const Variant&v);
    int16_r     to_int16(const Variant&v);
    int32_r     to_int32(const Variant&v);
    int64_r     to_int64(const Variant&v);
    integer_r   to_integer(const Variant&v);
    short_r     to_short(const Variant&v);
    //string_r    to_string(const Variant&v);
    uint8_r     to_uint8(const Variant&v);
    uint16_r    to_uint16(const Variant&v);
    uint32_r    to_uint32(const Variant&v);
    uint64_r    to_uint64(const Variant&v);
    unsigned_r  to_uinteger(const Variant&v);
    ushort_r    to_ushort(const Variant&v);
    #endif

}
