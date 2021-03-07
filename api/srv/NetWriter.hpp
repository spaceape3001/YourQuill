////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

//#include "yCommon.hpp"

//#include "db/Cache.hpp"
//#include "db/Enums.hpp"
//#include "util/Array2.hpp"
#include <util/Comma.hpp>
#include <util/Enum.hpp>
#include <util/List.hpp>
#include <util/Set.hpp>
#include <util/Vector.hpp>


#include <QByteArray>
#include <QTextStream>

class QDate;
class QDateTime;
class QDir;
class QUrl;
//class QVariant;
//class Page;
//class Class;
//class Graph;

class NetWriter {
public:

    const QByteArray&   title() const { return m_title; }
    void                title(const char*);
    void                title(const QByteArray&);
    void                title(const QString&);
    void                title(const String&);
    void                flush();
    const QByteArray&   bytes() const { return m_bytes; }
    QByteArray          steal();

    template <typename T>
    void    write(const T& data)
    {
        m_stream << data;
    }
    
    template <typename E>
    void    write(EnumImpl<E> v)
    {
        write(v.key());
    }
    
    template <typename T>
    void    write(const List<T>& values, const QString& sep = ", ")
    {
        Comma   comma(sep);
        for(const T& v : values){
            write(comma.text());
            write(v);
            ++comma;
        }
    }
    
    template <typename T>
    void    write(const Vector<T>& values, const QString& sep = ", ")
    {
        Comma   comma(sep);
        for(const T& v : values){
            write(comma.text());
            write(v);
            ++comma;
        }
    }

    template <typename T, typename C>
    void    write(const Set<T,C>& values, const QString& sep = ", ")
    {
        Comma   comma(sep);
        for(const T& v : values){
            write(comma.text());
            write(v);
            ++comma;
        }
    }
    
    void    write(bool);
    void    write(const QColor&);
    //void    write(const QDate&);
    //void    write(const QDateTime&);
    void    write(const QDir&);
    void    write(const QRect&);
    void    write(const QStringList&, const QString& sep=", ");
    void    write(const QUrl&);
    void    write(const String&);
    //void    write(const QVariant&);
    
    /*! \brief  Appends document to stream
    
        \param[in] documentname  Document to be appended
        \param[in] fSuppressMsg     Suppresses the log-message if TRUE
        
        \return TRUE if the document loaded successfull, FALSE otherwise
    */
    bool    append_document(const QString&documentname, bool fSuppressMsg=false);
    
protected:
    QByteArray  m_bytes;
    QTextStream m_stream;
    QByteArray  m_title;

    void        write_in(NetWriter&&);
    
    NetWriter();
    ~NetWriter();
};

