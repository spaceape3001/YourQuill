////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

    //  SKIPPING INCLUDES... done in order by others

namespace yq {

    inline Variant::Variant() : m_type(&invalid()), m_data{}
    {
    }


    template <typename T>
    Variant::Variant(T&& val) : m_type(nullptr)
    {
        set(val);
    }

    template <typename T>
    requires is_type_v<std::decay_t<T>>
    Variant&    Variant::operator=(T&& cp)
    {
        set(cp);
        return *this;
    }

    template <typename T>
    requires is_type_v<T>
    bool        Variant::operator==(const T&b) const
    {
        if(&meta<T>() != m_type)
            return false;
        return m_data.reference<T>() == b;
    }


    template <typename T>
    requires is_type_v<T>
    bool        operator==(const T& a, const Variant& b)
    {
        return b.operator==(a);
    }

    template <typename T>
    bool        Variant::can_convert() const
    {
        static_assert( is_type_v<T>, "TypeInfo T must be metatype defined!");
        return can_convert_to(&meta<T>());
    }

    template <typename T>
    Variant     Variant::convert() const
    {
        static_assert( is_type_v<T>, "TypeInfo T must be metatype defined!");
        return convert_to(meta<T>());
    }

    template <typename T>
    const T&            Variant::ref(const T& bad) const
    {
        static_assert( is_type_v<T>, "TypeInfo T must be metatype defined!");
        return (m_type == &meta<T>()) ?  m_data.reference<T>() : bad;
    }

    template <typename T>
    void Variant::set(T&& val)
    {
        using U     = std::decay<T>::type;
        static_assert( is_type_v<U>, "TypeInfo must be metatype defined!");
        
        const TypeInfo& newType  = meta<U>();
        if(m_type){
            if(&newType == m_type){
                if constexpr (std::is_rvalue_reference_v<T>){
                    m_data.reference<U>() = std::move(val);
                } else {
                    m_data.reference<U>() = val;
                }
                return;
            }
            (m_type->m_dtor)(m_data);
        }
        
        assert(good(newType));
        m_type      = &newType;
        if constexpr (std::is_rvalue_reference_v<T>){
            m_data.ctorMove(std::move(val));
        } else {
            m_data.ctorCopy(val);
        }
    }


    template <typename T>
    Result<T>   Variant::value() const
    {
        static_assert( is_type_v<T>, "TypeInfo T must be metatype defined!");
        if(m_type == &meta<T>())
            return m_data.reference<T>();
            
        auto cvt = m_type -> m_convert.get(&meta<T>(), nullptr);
        if(cvt){
            T       tmp;
            (*cvt)(&tmp, raw_ptr());
            return tmp;
        }
        return Result<T>();
    }
}
