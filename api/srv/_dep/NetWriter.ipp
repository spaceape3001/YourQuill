////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


QTextStream&    operator<<(QTextStream&str, const std::string_view&v)
{
    return str << QString::fromStdString(std::string(v));
}

QTextStream&    operator<<(QTextStream&str, const String&v)
{
    return str << QString::fromStdString(v);
}



QTextStream&    operator<<(QTextStream&str, const std::string&v)
{
    return str << QString::fromStdString(v);
}

QTextStream&    operator<<(QTextStream&str, const std::filesystem::path&v)
{
    return str << QString::fromStdString(v.string());
}


NetWriter::NetWriter() : NetWriter(NullStream::null())
{
}

NetWriter::NetWriter(Stream& str) : m_stream(&str)
{
}

NetWriter::~NetWriter()
{
}


bool    NetWriter::append_document(const std::filesystem::path& documentname, bool fSuppressMsg)
{
    if(m_stream){
        
        QFile   document(QString::fromStdString(documentname.string()));
        if(!document.open(QIODevice::ReadOnly)){
            if(!fSuppressMsg)
                srvError << "Unable to read document " << documentname;
            return false;
        }
        
        *m_stream << String(document.readAll());
    }
    document.close();
    return true;
}


//QByteArray          NetWriter::steal()
//{
    //return QByteArray(std::move(m_bytes));
//}

void    NetWriter::title(const std::string_view&z)
{
    m_title = z;
}

//void    NetWriter::title(const QByteArray&z)
//{
    //m_title = z;
//}

//void    NetWriter::title(const QString&z)
//{
    //m_title = utf8(z);
//}

//void    NetWriter::title(const String&z)
//{
    //m_title = z.c_str();
//}

void    NetWriter::write(bool f)
{
    chk();
    *m_stream << (f?"Yes":"No");
}

void    NetWriter::write(const String&s)
{
    chk();
    *m_stream << s.qString();
}

void    NetWriter::write(const QColor&v)
{
    chk();
    if(v.isValid())
        *m_stream << v.name();
    else
        *m_stream << "(invalid)";
}

//void    NetWriter::write(const QDate& dt)
//{
//    chk();
    //m_stream << toGUIString(dt);
//}

//void    NetWriter::write(const QDateTime&dt)
//{
//    chk();
    //m_stream << toGUIString(dt);
//}

void    NetWriter::write(const QDir& dir)
{
    *m_stream << dir.absolutePath();
}

void    NetWriter::write(const QRect&r)
{
    char    buffer[256];
    sprintf(buffer, "%dx%d%+d%+d", r.width(), r.height(), r.left(), r.top());
    *m_stream << buffer;
}

void    NetWriter::write(const QUrl&url)
{
    *m_stream << url.toString();
}

void    NetWriter::write(const QStringList&data, const String& sep)
{
    *m_stream << data.join(sep.qString());
}

//void    NetWriter::write(const QVariant&var)
//{
//    chk();
    //m_stream << variantToIOString(var);
//}

