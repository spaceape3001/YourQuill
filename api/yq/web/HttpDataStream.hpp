////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/stream/Stream.hpp>

#include "HttpData.hpp"
#include <functional>

namespace yq {
    class HttpDataStream : public Stream {
    public:
    
        using DestinationX   = std::function<void(HttpDataPtr)>;
    
        HttpDataStream(std::vector<HttpDataPtr>& destination);
        HttpDataStream(DestinationX);
        HttpDataStream(HttpDataPtr);
        ~HttpDataStream();

        void close() override;
        void flush() override;
        bool is_open() const override;
        bool write(const char* z, size_t cb) override;
        bool is_full() const;
        
        template <typename T>
        Stream& operator<<(T v)
        {
            return ((Stream&) *this) << v;
        }
        
    private:
        DestinationX    m_destination;
        HttpDataPtr     m_buffer;
        
        HttpDataPtr&    buffer();
    };
}
