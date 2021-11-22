////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <QString>
#include "String.hpp"

class Comma {
public:
    Comma(const String&sep=", ") : m_separator(sep), m_first(true) {}
    
    const String&     text() const 
    {
        static String s_blank;
        return m_first ? s_blank : m_separator;
    }
    
    QString             qText() const
    {
        return text().qString();
    }
    
    Comma& operator++()
    {
        m_first = false;
        return *this;
    }
    
private:
    String      m_separator;
    bool        m_first;
};
