////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/Stream.hpp>
class ByteArray;

namespace yq {
    namespace stream {
        class Bytes : public Stream {
        public:
            Bytes(ByteArray& bytes) : m_bytes(bytes) {}
            ~Bytes(){}

            bool write(const char* buf, size_t cb) override;

        private:
            ByteArray&  m_bytes;
        };
    }
}
