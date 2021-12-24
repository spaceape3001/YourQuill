#pragma once
#include "Stream.hpp"

/*! \brief Line Counter for a stream

    Wrap this around another stream to get a count of line/columns
*/
class LineColStream : public Stream {
public:
    LineColStream(Stream& out) : m_stream(out) {}
    
    unsigned int    line() const { return m_line; }
    unsigned int    column() const { return m_column; }
    
    bool    write(const char* z, size_t cb) override
    {
        for(size_t i=0;i<cb;++i){
            if(z[i] == '\n'){
                ++m_line;
                m_column    = 0;
            } else
                ++m_column;
        }
        return m_stream.write(z, cb);
    }
    
private:
    Stream&             m_stream;
    unsigned int        m_line      = 0;
    unsigned int        m_column    = 0;
};
