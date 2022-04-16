#pragma once

#include <algorithm>
#include <string_view>
#include <yq/stream/Stream.hpp>

namespace yq {

    template <size_t N>
    class FixedBuffer : public Stream {
    public:
    
        static constexpr const size_t  SIZE    = N;
    
        static consteval size_t size() { return N; }
        
        void    advance(size_t n)
        {
            m_count = std::min(m_count+n, N);
        }
        
        void    append(const char*z, size_t cnt)
        {
            if(!z || !cnt)
                return ;
            if(m_count >= N)
                return ;
                
            cnt     = std::min(N-m_count, cnt);
            memcpy(m_buffer+m_count, z, cnt);
            m_count += cnt;
        }
        
        void    append(std::string_view sv)
        {
            advance(sv.data(), sv.size());
        }
        
        std::string_view    as_view() const
        {
            return std::string_view(m_buffer, m_count);
        }
        
        std::string_view    as_view(size_t n)
        {
            if(n>=m_count)
                n   = m_count;
            return std::string_view(m_buffer+n, m_count-n);
        }
        
        //! Number of free bytes available
        size_t  available() const { return N - m_count; }
        char*       data() { return m_buffer; }
        const char* data() const { return m_buffer; }
        bool    empty() const { return !m_count; }
        bool    full() const { return m_count >= N; }

        char*   freespace()  { return m_buffer + m_count; }

        virtual bool    is_open() const { return true; }

        virtual bool    write(const char*z, size_t cb) override
        {
            append(z, cb);
            return true;
        }
        
        size_t  count() const { return m_count; }
        
        void    count(size_t n)
        {
            m_count = std::min(n, N);
        }
        
    private:
        size_t  m_count       = 0;
        char    m_buffer[N];
    };

}
