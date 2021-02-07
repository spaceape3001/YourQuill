////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CoreFwd.hpp"

template <typename T>
class Touched {
public:
    constexpr Touched(const T& v = {}) : m_value(v), m_touched(false) {}
    
    Touched& operator=(const T& v)
    {
        m_value =    v;
        m_touched   = true;
        return *this;
    }
    
    operator const T&  () const { return m_value; }

    const T&    get() const { return m_value; }

    void    reset() { m_touched = false; }
    bool    touched() const { return m_touched; }
private:
    T       m_value;
    bool    m_touched;
};


