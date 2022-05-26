////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Stream.hpp>

namespace yq {
    class String;
    
    namespace stream {
        class Text : public Stream {
        public:
            Text(std::string& bytes) : m_bytes(bytes) {}
            ~Text(){}

            bool write(const char* buf, size_t cb) override;

        private:
            std::string&  m_bytes;
        };
    }
}
