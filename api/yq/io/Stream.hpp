#pragma once

#include "yq/util/String.hpp"

class String;
template <typename T> class Vector;

class Stream {
public:
    
    Stream(Vector<char>& buf) : m_buffer(buf), m_line(1), m_col(0) {}
    
    unsigned int    line() const { return m_line; }
    unsigned int    column() const { return m_col; }
    
    Stream&     operator<<(char ch)
    {
        put(ch);
        return *this;
    }
    
    Stream&     operator<<(const std::string& s)
    {
        for(char ch : s)
            put(ch);
        return *this;
    }
    
    Stream&     operator<<(const std::string_view& s)
    {
        for(char ch : s)
            put(ch);
        return *this;
    }
    
    Stream&     operator<<(const char* z)
    {
        for(; z && *z; ++z)
            put(*z);
        return *this;
    }

    
    void        put(char ch)
    {
        m_buffer << ch;
        
        if(ch == '\n'){
            ++ m_line;
            m_col   = 0;
        } else {
            ++m_col;
        }
    }

private:
    Vector<char>&           m_buffer;
    unsigned int            m_line, m_col;
    
};
