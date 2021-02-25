////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <QString>

class Comma {
public:
    Comma(const QString&sep=", ") : m_separator(sep), m_first(true) {}
    
    const QString&     text() const 
    {
        static QString s_blank;
        return m_first ? s_blank : m_separator;
    }
    
    Comma& operator++()
    {
        m_first = false;
        return *this;
    }
    
private:
    QString     m_separator;
    bool        m_first;
};
