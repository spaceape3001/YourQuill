#pragma once

//! Abstract baseis for a stream, yes it'll use virtuals
class Stream {
public:
    virtual ~Stream(){}
    virtual void    write(const char*, size_t) = 0;
};

