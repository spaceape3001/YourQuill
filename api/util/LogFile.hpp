////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QFile>
#include <QTextStream>

struct LogFile {
    QString         m_fpath;
    QFile           m_file;
    QTextStream     m_txt;
    
    LogFile(const QString& path);
    
    struct Line;
    Line    line();
};


struct LogFile::Line {
    QTextStream*    m_stream;
    
    explicit Line(QTextStream* str);
    Line(const Line&) = delete;
    Line(Line&& mv);
    Line& operator=(const Line&) = delete;
    Line& operator=(Line&& mv);
    
    ~Line();
    
    template <typename T>
    Line& operator<<(const T& d)
    {
        if(m_stream)
            *m_stream << d;
        return *this;
    }
};
