#pragma once

FileStream::FileStream()
{
}

FileStream::~FileStream()
{
    close();
}

bool        FileStream::open(const std::filesystem::path& pth)
{
    if(m_file.is_open())
        return false;
    m_file.open(pth, std::ios_base::binary);
    if(m_file){
        m_path      = pth;
        return true;
    } else
        return false;
}

void        FileStream::close()
{
    if(m_file.is_open()){
        m_path.clear();
        m_file.flush();
        m_file.close();
    }
}

bool        FileStream::is_open() const
{
    return m_file.is_open();
}

bool    FileStream::write(const char*z, size_t cb) 
{
    if(m_file.is_open()){
        m_file.write(z, cb);
        return true;
    } else
        return false;
}

void    FileStream::flush() 
{
    if(m_file.is_open())
        m_file.flush();
}

