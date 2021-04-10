////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


FileName::FileName() = default;
FileName::FileName(const FileName&) = default;
FileName::FileName(FileName&&) = default;
FileName::~FileName() = default;
FileName& FileName::operator=(const FileName&) = default;
FileName& FileName::operator=(FileName&&) = default;

FileName::FileName(const QString& s)
{
    _set(s);
}

FileName&     FileName::operator=(const QString&s)
{
    _set(s);
    return *this;
}

void    FileName::_set(const QString&fname)
{
    m_full  = fname;
    if(fname.isEmpty()){
        m_name  = m_path    = QString();
    } else {
        QFileInfo   fi(fname);
        m_name      = fi.fileName();
        m_path      = fi.absolutePath();
    }
}

void    FileName::clear()
{
    m_full.clear();
    m_name.clear();
    m_path.clear();
}
