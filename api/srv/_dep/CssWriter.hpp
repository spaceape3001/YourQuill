////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "NetWriter.hpp"

namespace yq {

    class CssWriter : public NetWriter {
    public:
        CssWriter(Stream&);
        ~CssWriter();
        
        template <typename T>
        CssWriter& operator<<(const T& v)
        {
            write(v);
            return *this;
        }
    };
}
