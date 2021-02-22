////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "LogFile.hpp"
#include "Logging.hpp"
#include "yq/util/TypeFwd.hpp"
#include <QTimer>

LogFile::LogFile(const QString& path) : 
    m_fpath(path.arg(thread_id())),
    m_file(m_fpath), 
    m_txt(&m_file)
    
{
    m_file.open(QIODevice::Append | QIODevice::Text | QIODevice::WriteOnly);
    yInfo() << "Starting logger " << m_fpath;
    
    QTimer*t        = new QTimer(&m_file);
    t->setInterval(5);  // flush every 5ms
    QObject::connect(t, &QTimer::timeout, [this](){  
        m_txt.flush(); 
        m_file.flush();
    });
    t->start();
}


LogFile::Line LogFile::line()
{
    return Line(&m_txt);
}


LogFile::Line::Line(QTextStream* str)  : m_stream(str)
{
}

LogFile::Line::Line(Line&& mv) 
{
    m_stream      = mv.m_stream;
    mv.m_stream   = nullptr;
}

LogFile::Line& LogFile::Line::operator=(Line&& mv)
{
    if(this != &mv){
        if(m_stream)
            *m_stream << "\n";
        m_stream        = mv.m_stream;
        mv.m_stream     = nullptr;
    }
    return *this;
}

LogFile::Line::~Line()
{
    if(m_stream)
        *m_stream << "\n";
}
