#pragma once
#include "Stream.hpp"
#include "Vector.hpp"
#include <string_view>

struct BufferStream : public Stream {
    Vector<char>        data;
    
    BufferStream(size_t nRes=4096) 
    { 
        data.reserve(nRes); 
    }
    
    ~BufferStream(){}
    
    void write(const char* buf, size_t cb) override
    {
        data.append(buf, cb);
    }
    
    std::string_view view() const 
    {
        return std::string_view(data.data(), data.size());
    }
};

