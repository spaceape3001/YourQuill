////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Enum.hpp"
#include <assert.h>

/*! \brief A flag object for enums (up to 64-values)
*/
template <typename E, typename T=uint64_t>
class Flag {
public:
    using DefEnum   = E;
    using DefType   = T;
    using enum_t    = typename E::enum_t;

    static T mask(enum_t e)
    {
        #ifdef _DEBUG
        assert((e>=0) && (e < 8*sizeof(T)) && "Enum value out of range!");
        #endif
        
        return T(1) << e;
    }
    
    static Flag all()
    {
        return Flag(~T(0));
    }
    

    //  Default ....
    Flag() : m_value(0) {}
    
    Flag(std::initializer_list<enum_t> flags) : m_value(0)
    {
        for(enum_t e : flags)
            m_value |= mask(e);
    }
    
    Flag(enum_t e) : m_value(mask(e)) {}

    Flag(T v) : m_value(v) {}
    
    T       value() const { return m_value; }
    
    Flag        operator+(enum_t e) const
    {
        return Flag(m_value | mask(e));
    }

    Flag        operator+(Flag f) const
    {
        return Flag(m_value | f.m_value );
    }
    
    Flag&       operator += (enum_t e)
    {
        m_value |= mask(e);
        return *this;
    }

    Flag        operator-(enum_t e) const
    {
        return Flag(m_value & ~mask(e));
    }
    
    Flag        operator-(Flag f) const
    {
        return Flag(m_value & ~f.m_value);
    }

    Flag&       operator -= (enum_t e)
    {
        m_value &= ~mask(e);
        return *this;
    }
    
    Flag        operator~() const
    {
        return Flag(~m_value);
    }
    
    Flag       operator |(enum_t e) const
    {
        return Flag(m_value | mask(e));
    }
    
    Flag&      operator |=(enum_t e)
    {
        m_value |= mask(e);
        return *this;
    }

    Flag       operator |(Flag f) const
    {
        return Flag(m_value | f.m_value);
    }

    Flag&      operator |=(Flag f)
    {
        m_value |= f.m_value;
        return *this;
    }

    Flag        operator&(enum_t e) const
    {
        return Flag(m_value & mask(e));
    }
    
    Flag&      operator &=(enum_t e)
    {
        m_value &= mask(e);
        return *this;
    }

    Flag       operator &(Flag f) const
    {
        return Flag(m_value & f.m_value);
    }
 
    Flag&      operator &=(Flag f)
    {
        m_value &= f.m_value;
        return *this;
    }

    Flag        operator^(enum_t e) const
    {
        return Flag(m_value ^ mask(e));
    }
    
    Flag&      operator ^=(enum_t e)
    {
        m_value ^= mask(e);
        return *this;
    }

    Flag       operator ^(Flag f) const
    {
        return Flag(m_value ^ f.m_value);
    }

    Flag&      operator ^=(Flag f)
    {
        m_value ^= f.m_value;
        return *this;
    }
    
    bool    operator==(const Flag& b) const
    {
        return m_value == b.m_value;
    }

    bool    operator!=(const Flag& b) const
    {
        return m_value != b.m_value;
    }

    bool is_set(enum_t e) const
    {
        return (m_value & mask(e))?true:false;
    }
    
    bool is_clear(enum_t e) const
    {
        return (m_value & mask(e))?true:false;
    }
    
    bool    operator[](enum_t e) const
    {
        return is_set(e);
    }
    

private:
    T    m_value;
};

template <typename E>
Flag<E>     all_set()
{
    return Flag<E>::all();
}

