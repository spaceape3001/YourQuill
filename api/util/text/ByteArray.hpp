////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <util/collection/Vector.hpp>

class QByteArray;

namespace yq {
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
        
        auto    begin() { return m_data.begin(); }
        auto    begin() const { return m_data.begin(); }
        auto    cbegin() const { return m_data.cbegin(); }
        auto    end() { return m_data.end(); }
        auto    end() const { return m_data.end(); }
        auto    cend() { return m_data.cend(); }
        
        auto    rbegin() { return m_data.begin(); }
        auto    rbegin() const { return m_data.begin(); }
        auto    crbegin() const { return m_data.cbegin(); }
        
        auto    rend() { return m_data.end(); }
        auto    rend() const { return m_data.end(); }
        auto    crend() { return m_data.cend(); }

        ByteArray&      append(const char* z, size_t cb)
        {
            m_data.append(z, cb);
            return *this;
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
        
        ByteArray&  operator<<(char ch)
        {
            m_data += ch;
            return *this;
        }
        
        
    private:
        // this is binary data BTW, no interpretation
        Vector<char>        m_data;
    };
}
