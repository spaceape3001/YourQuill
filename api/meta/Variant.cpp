////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "MetaField.hpp"
#include "MetaValue.hpp"
#include "Variant.hpp"

namespace yq {
    namespace {
        
        const MetaValue& mtString    = metaValue<String>();
        const MetaValue& mtBoolean   = metaValue<bool>();
        const MetaValue& mtNumber    = metaValue<double>();
        const MetaValue& mtInteger   = metaValue<int64_t>();
        const MetaValue& mtUInteger  = metaValue<uint64_t>();

    }

    Variant::Variant() : m_type(&MetaValue::invalid())
    {
    }

    Variant::Variant(const Variant&cp) : m_type(cp.m_type)
    {
        (m_type -> m_ctorCopyB)(m_data, cp.m_data);
    }

    Variant::Variant(const MetaValue&mt) : m_type(&mt)
    {
        (m_type -> m_ctorCopyB)(m_data, m_type -> m_defCopy);
    }

    Variant::Variant(const MetaValue*mt) : Variant(mt ? *mt : MetaValue::invalid())
    {
    }


    Variant::Variant(const MetaValue&mt, const void*p) : m_type(&mt)
    {
        (m_type->m_ctorCopyR)(m_data, p);
    }


    Variant::Variant(Variant&& mv) : m_type(mv.m_type)
    {
        (m_type -> m_ctorMove)(m_data, std::move(mv.m_data));
    }

    Variant::~Variant()
    {
        (m_type -> m_dtor)(m_data);
    }


    Variant&    Variant::operator=(const Variant&cp)
    {
        if(&cp != this){
            if(cp.m_type == m_type){
                (m_type -> m_copyB)(m_data, cp.m_data);
            } else {
                (m_type -> m_dtor)(m_data);
                m_type  = cp.m_type;
                (m_type -> m_ctorCopyB)(m_data, cp.m_data);
            }
        }
        return *this;
    }

    Variant&    Variant::operator=(Variant&&mv)
    {
        if(&mv != this){
            if(mv.m_type == m_type){
                (m_type -> m_moveB)(m_data, std::move(mv.m_data));
            } else {
                (m_type -> m_dtor)(m_data);
                m_type  = mv.m_type;
                (m_type -> m_ctorMove)(m_data, std::move(mv.m_data));
            }
        }
        return *this;
    }

    bool        Variant::operator==(const Variant&b) const
    {
        if(this == &b)
            return true;
        if(m_type != b.m_type)
            return false;
        return (m_type -> m_equal)(m_data, b.m_data);
    }

    bool        Variant::operator!=(const Variant&b) const
    {
        return !operator==(b);
    }

    bool        Variant::can_convert_to(const MetaValue& mt) const
    {
        if(&mt == m_type)
            return true;
        if(&mt == &MetaValue::invalid())
            return true;
        if(&mt == &MetaValue::variant())
            return true;
        return m_type -> has_conversion(mt);
    }

    Variant     Variant::convert_to(const MetaValue& mt) const
    {
        if(m_type == &mt)
            return *this;
        if(m_type == &MetaValue::invalid())
            return Variant();
        if(m_type == &MetaValue::variant())
            return *this;
            
        auto fn = m_type -> converter(mt);
        if(!fn)
            return Variant();
        
        Variant ret(mt);
        fn(ret.ptr(), ptr());
        return ret;
    }

    Variant     Variant::get_field(const String&k) const
    {
        const MetaField*    mf  = m_type -> field(k);
        if(!mf)
            return Variant();
        return mf -> get(ptr());
    }

    String   Variant::io_format() const
    {
        if(!m_type -> m_ioFormat)
            return String();
        return (m_type -> m_ioFormat)(ptr());
    }

    Variant  Variant::io_parse(const MetaValue&mt, const String&str)
    {
        if(!mt.m_ioParse)
            return Variant();
            
        Variant     ret(mt);
        bool    f   = (mt.m_ioParse)(str, ret.ptr());
        if(f)
            return ret;
        return Variant();
    }

    bool        Variant::is_valid() const
    {
        return m_type && (m_type != &MetaValue::invalid());
    }


    String      Variant::print() const
    {
        if(m_type -> m_print){
            return (m_type->m_print)(ptr());
        } else
            return "(unprintable)";
    }

    bool        Variant::set_field(const String&k, const Variant&v)
    {
        const MetaField*    mf  = m_type -> field(k);
        if(!mf)
            return false;
        if(v.type() == mf -> value())
            return mf -> set(ptr(), v.ptr());
        Variant v2  = v.convert_to(mf->value());
        if(!v2.is_valid())
            return false;
        return mf -> set(ptr(), v2.ptr());
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //      SPECIALIZED CONSTRUCTORS
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        Variant::Variant(char ch) : Variant(String(ch))
        {
        }

        Variant::Variant(char8_t ch) : Variant(String(ch))
        {
        }

        Variant::Variant(char32_t ch) : Variant(String(ch))
        {
        }

        Variant::Variant(const char* z) : Variant(String(z))
        {
        }

        Variant::Variant(const char8_t* z) : Variant(String(z))
        {
        }

        Variant::Variant(const char32_t* z) : Variant(String(z))
        {
        }

        Variant::Variant(const std::string&z) : Variant(String(z))
        {
        }

        Variant::Variant(const std::u8string&z) : Variant(String(z))
        {
        }

        Variant::Variant(const std::u32string&z) : Variant(String(z))
        {
        }

        Variant::Variant(const std::wstring&z) : Variant(String(z))
        {
        }

        Variant::Variant(const String& cp) : m_type(&mtString)
        {
            m_data.ctorCopy(cp);
        }



}

