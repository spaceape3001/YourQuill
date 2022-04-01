////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <assert.h>
#include <stdint.h>
#include <unistd.h>
#include <string>
#include <string_view>

namespace yq {
    namespace ipc {
        enum class ReadResult {
            Read,
            Empty,
            Error
        };


        template <size_t N>
        struct Buffer {
            size_t  m_len;
            char    m_data[N];
            
            Buffer() : m_len{0} {}
            
            void        add_string(const char* z)
            {
                while(*z && (m_len < N))
                    m_data[m_len++] = *z++;
            }
            
            void        add_string(const char* z, size_t s)
            {
                for(; s && *z && (m_len < N); --s, ++z, ++m_len)
                    m_data[m_len] = *z;
            }
            
            template <typename T>
            T*          add()
            {
                if(m_len + sizeof(T) > N)
                    return nullptr;
                T*  p   = (T*)(m_data + m_len);
                m_len += sizeof(T);
                return p;
            }
            
            Buffer& operator<<(const std::string_view& v)
            {
                add_string(v.data(), v.size());
                return *this;
            }

            Buffer& operator<<(const std::string& v)
            {
                add_string(v.c_str());
                return *this;
            }

            template <typename T>
            T*          add(const T& v)
            {
                T*  p   = add<T>();
                *p      = v;
                return p;
            }

            consteval size_t capacity() const { return N; }
            
            void        clear()
            {
                m_len = 0;
            }
            
            bool        empty() const 
            {
                return m_len == 0;
            }
            
            bool        full() const
            {
                return m_len >= N;
            }

            
            bool        half_empty() const
            {
                return m_len <= (N>>1);
            }
            
            bool        half_full() const
            {
                return m_len >= (N>>1);
            }
            

            ReadResult    read_from(int fd)
            {
                ssize_t l   = ::read(fd, m_data, N);
                if((l < 0) && (errno != EAGAIN))
                    return ReadResult::Error;
                if(l<=0){
                    m_len   = 0;
                    return ReadResult::Empty;
                }
                m_len       = (size_t) l;
                return ReadResult::Read;
            }
            
            size_t  size() const 
            { 
                return m_len; 
            }
            
            void    write_to(int fd)
            {
                if(m_len){
                    (void) !::write(fd, m_data, m_len);
                    m_len       = 0;
                }
            }
            
            const char* begin() const { return m_data; }
            const char* end() const { return m_data + m_len; }
            const char* cbegin() const { return m_data; }
            const char* cend() const { return m_data + m_len; }
            char*       begin() { return m_data; }
            char*       end() { return m_data + m_len; }
            
            char        operator[](size_t n) const 
            {
                assert(n<N);
                if(n>=N)
                    return 0x00;
                return m_data[n];
            }
            
            
        } __attribute__ ((aligned(__alignof__(uint64_t))));
    }
}
