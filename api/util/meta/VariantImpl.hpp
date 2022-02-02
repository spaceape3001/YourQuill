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
    void Variant::set(T&& val)
    {
        using U     = std::decay<T>::type;
        static_assert( is_type_v<U>, "TypeInfo must be metatype defined!");
        m_type      = &meta<U>();
        if constexpr (std::is_rvalue_reference_v<T>){
            m_data.ctorMove(std::move(val));
        } else {
            m_data.ctorCopy(val);
        }
    }

    template <typename T>
    Variant::Variant(T&& val)
    {
        set(val);
    }

    template <typename T>
    bool        Variant::operator==(const T&b) const
    {
        static_assert( is_type_v<T>, "TypeInfo T must be metatype defined!");
        if(&meta<T>() != m_type)
            return false;
        return m_data.reference<T>() == b;
    }


    template <typename T>
    bool        operator==(const T& a, const Variant& b)
    {
        return b.operator==(a);
    }



#if 0
    template <typename T>
    bool        Variant::can_convert_to() const
    {
        static_assert( is_type<T>, "TypeInfo T must be metatype defined!");
        return can_convert_to(metaType<T>());
    }

    template <typename T>
    Variant     Variant::convert_to() const
    {
        static_assert( is_type<T>, "TypeInfo T must be metatype defined!");
        return convert_to(metaType<T>());
    }

    template <typename T>
    Result<T>   Variant::value() const
    {
        static_assert( is_type<T>, "TypeInfo T must be metatype defined!");
        if(m_type == &metaType<T>())
            return m_data.reference<T>();
        auto cvt = m_type -> m_convert.get(&metaType<T>(), nullptr);
        if(cvt){
            T       tmp;
            (*cvt)(&tmp, ptr());
            return tmp;
        }
        return Result<T>();
    }
#endif
    
}
