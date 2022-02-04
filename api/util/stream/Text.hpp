////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/stream/Stream.hpp>

namespace yq {
    class String;
    
    namespace stream {
        class Text : public Stream {
        public:
            Text(String& bytes) : m_bytes(bytes) {}
            ~Text(){}

            bool write(const char* buf, size_t cb) override;

        private:
            String&  m_bytes;
        };
    }
}
