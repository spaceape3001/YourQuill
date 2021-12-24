#pragma once

#include "ByteArray.hpp"
#include "Stream.hpp"

class ByteStream : public Stream {
public:
    ByteStream(ByteArray& bytes) : m_bytes(bytes) {}
    ~ByteStream(){}

    bool write(const char* buf, size_t cb) override
    {
        m_bytes.append(buf, cb);
        return true;
    }

private:
    ByteArray&  m_bytes;
};
