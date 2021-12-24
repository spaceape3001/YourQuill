#pragma once

#include <QByteArray>
#include "util/Stream.hpp"

struct QByteArrayStream : public Stream {
    QByteArray&          data;
    
    QByteArrayStream(QByteArray& ref) : data(ref) {}
    
    void write(const char* buf, size_t cb) override
    {
        data.append(buf, cb);
    }
};
