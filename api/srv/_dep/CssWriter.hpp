////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "NetWriter.hpp"

class CssWriter : public NetWriter {
public:
    CssWriter();
    ~CssWriter();
    
    template <typename T>
    CssWriter& operator<<(const T& v)
    {
        write(v);
        return *this;
    }
};
