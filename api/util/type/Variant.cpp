#include "Variant.hpp"
#include <util/text/String.hpp>


namespace yq {
    Variant::Variant(const Variant& cp) : m_type(cp.m_type)
    {
        assert(m_type);
        assert(m_type -> m_ctorCopyB);

        (m_type -> m_ctorCopyB)(m_data, cp.m_data);
    }
    
    Variant::Variant(Variant&&mv) : m_type(mv.m_type)
    {
        (m_type -> m_ctorMove)(m_data, std::move(mv.m_data));
        mv.m_type   = &invalid();
    }
    
    Variant::Variant(char ch)
    {
        set(String(ch));
    }
    
    Variant::Variant(char8_t ch)
    {
        set(String(ch));
    }
    
    Variant::Variant(char32_t ch)
    {
        set(String(ch));
    }
    
    Variant::Variant(const char*z)
    {
        set(String(z));
    }
    
    Variant::Variant(const char8_t*z)
    {
        set(String(z));
    }
    
    Variant::Variant(const char32_t*z)
    {
        set(String(z));
    }
    
    Variant::Variant(const std::string&z)
    {
        set(String(z));
    }
    
    Variant::Variant(const std::u8string&z)
    {
        set(String(z));
    }
    
    Variant::Variant(const std::u32string&z)
    {
        set(String(z));
    }
    
    Variant::Variant(const std::wstring&z)
    {
        set(String(z));
    }
    
    Variant::Variant(const String&z)
    {
        set(z);
    }
    

#ifdef ENABLE_QT
    Variant::Variant(const QString&z)
    {
        set(String(z));
    }
#endif
    
    
    //! Creates a defaulted Variant to the specified meta-type
    Variant::Variant(const TypeInfo&mt) : m_type(&mt)
    {
        assert(m_type);
        assert(m_type -> m_ctorCopyB);

        (m_type -> m_ctorCopyB)(m_data, m_type -> m_default);
    }
    
    Variant::Variant(const TypeInfo&mt, const void* dp) : m_type(&mt)
    {
        assert(m_type);
        assert(m_type -> m_ctorCopyR);
        (m_type -> m_ctorCopyR)(m_data, dp);
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
                assert(m_type -> m_moveB);

                (m_type -> m_moveB)(m_data, std::move(mv.m_data));
            } else {
                assert(m_type -> m_dtor);
                assert(m_type -> m_ctorMove);
                
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
        assert(m_type);
        assert(m_type -> m_equal);
    
        if(this == &b)
            return true;
        if(m_type != b.m_type)
            return false;
        return (m_type->m_equal)(m_data, b.m_data);
    }
    

    bool            Variant::can_convert_to(const TypeInfo&t) const
    {
        assert(m_type);
        if(m_type == &t)
            return true;
        return m_type->m_convert.has(&t);
    }

    const void*      Variant::raw_ptr() const 
    {
        return m_type -> is_small() ? (const void*) m_data.Data : m_data.Pointer;
    };
    
    void*            Variant::raw_ptr() 
    {
        return m_type -> is_small() ? (void*) m_data.Data : m_data.Pointer;
    };
}
