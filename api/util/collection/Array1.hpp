////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/collection/Vector.hpp>

namespace yq {

    template <typename T>
    class Array1 {
    public:

        enum { DIMS=1 };

        Array1() : m_zero{} {}
        ~Array1(){}
        
        T&          operator[](ssize_t z) { return m_data[(size_t)(m_zero + z)]; }
        const T&    operator[](ssize_t z) const { return m_data[(size_t)(m_zero + z)]; }
        T&          operator()(ssize_t z) { return m_data[(size_t)(m_zero + z)]; }
        const T&    operator()(ssize_t z) const { return m_data[(size_t)(m_zero + z)]; }
        
        auto        begin() { return m_data.begin(); }
        auto        begin() const { return m_data.begin(); }
        auto        cbegin() const { return m_data.cbegin(); }
        auto        end() { return m_data.end(); }
        auto        end() const { return m_data.end(); }
        auto        cend() const { return m_data.cend(); }
        
        void        clear() 
        { 
            m_data.clear(); 
            m_zero = 0; 
        }
        
        void        fill(const T& t)
        {
            for(auto& v : m_data)
                v  =  t;
        }
        
        size_t      size() const { return m_data.size(); }

        void        resize(ssize_t low, ssize_t high) 
        {
            m_zero          = -low;
            m_data.resize(high - low);
        }

        void        resize(ssize_t low, ssize_t high, const T& d) 
        {
            m_zero          = -low;
            m_data.resize(high - low, d);
        }

        ssize_t         low() const { return -m_zero; }
        ssize_t         high() const { return m_data.size() - m_zero; }


    private:
        ssize_t         m_zero;
        Vector<T>       m_data;
    };


}

