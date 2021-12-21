#pragma once
#include "Vector.hpp"

class QByteArray;

class ByteArray {
public:

    ByteArray() = default;
    ByteArray(const ByteArray&) = default;
    ByteArray(ByteArray&&) = default;
    
    ByteArray(const QByteArray&);
    ByteArray(const char*, size_t);
    ByteArray(const Vector<char>&);
    ByteArray(Vector<char>&&);
    
    ByteArray&  operator=(const ByteArray&) = default;
    ByteArray&  operator=(ByteArray&&) = default;
    
    ~ByteArray() = default;
    
    
    char&           operator[](size_t n) 
    { 
        return m_data[n]; 
    }

    char            operator[](size_t n) const 
    { 
        return m_data[n]; 
    }
    
    char*           data() 
    { 
        return m_data.data(); 
    }
    
    const char*     data() const 
    { 
        return m_data.data(); 
    }
    
    bool            empty() const 
    { 
        return m_data.empty(); 
    }
    
    QByteArray      qBytes() const;

    void            reserve(size_t z) 
    { 
        m_data.reserve(z); 
    }

    
    void            resize(size_t z)
    {
        m_data.resize(z);
    }
    
    void            setData(Vector<char>&&);
    
    size_t          size() const 
    { 
        return m_data.size(); 
    }
    
    
    
private:
    // this is binary data BTW, no interpretation
    Vector<char>        m_data;
};
