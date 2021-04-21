////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

PidFile::PidFile(const String& f, bool autoMake)
{
    m_file  = f;
    m_first = !file_exists(m_file.c_str());
    if(m_first && autoMake)
        make();
    
}

void     PidFile::make()
{
    std::ofstream   p(m_file);
    p << getpid();
}

PidFile::~PidFile()
{
    if(m_first){
        remove(m_file.c_str());
    }
}

