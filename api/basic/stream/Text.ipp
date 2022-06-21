////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Text.hpp"

namespace yq {
    namespace stream {
        bool Text::write(const char* buf, size_t cb) 
        {
            m_bytes.append(buf, cb);
            return true;
        }
    }
}
