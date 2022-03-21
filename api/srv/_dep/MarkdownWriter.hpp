////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "NetWriter.hpp"

class MarkdownWriter : public NetWriter {
public:

    struct CT {
        QByteArray  content, title;
    };
    
    static CT exec(const QByteArray&);

    MarkdownWriter(Stream&);
    ~MarkdownWriter();

    template <typename T>
    MarkdownWriter& operator<<(const T& v)
    {
        write(v);
        return *this;
    }
};

