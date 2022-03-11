////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "NetWriter.hpp"

class TextWriter : public NetWriter {
public:
    TextWriter();
    ~TextWriter();
    
    template <typename T>
    TextWriter& operator<<(const T& v)
    {
        write(v);
        return *this;
    }
};
