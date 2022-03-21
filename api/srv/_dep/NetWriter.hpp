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
#include <yq/Comma.hpp>
#include <yq/Enum.hpp>
#include <yq/List.hpp>
#include <yq/Set.hpp>
#include <yq/Vector.hpp>


#include <filesystem>

class QColor;
class QDate;
class QDateTime;
class QDir;
class QRect;
class QUrl;
//class QVariant;
//class Page;
//class Class;
//class Graph;
namespace yq { class Stream; }

//QTextStream&    operator<<(QTextStream&, const std::string_view&);
//QTextStream&    operator<<(QTextStream&, const std::string&);
//QTextStream&    operator<<(QTextStream&, const String&);
//QTextStream&    operator<<(QTextStream&, const std::filesystem::path&);

class NetWriter {
public:

    const String&       title() const { return m_title; }
    void                title(const std::string_view&);
    
    //String              steal();

    template <typename T>
    void    write(const T& data)
    {
        *m_stream << data;
    }
    
    template <typename E>
    void    write(EnumImpl<E> v)
    {
        write(v.key());
    }
    
    template <typename T>
    void    write(const List<T>& values, const String& sep = ", ")
    {
        Comma   comma(sep);
        for(const T& v : values){
            write(comma.text());
            write(v);
            ++comma;
        }
    }
    
    template <typename T>
    void    write(const Vector<T>& values, const String& sep = ", ")
    {
        Comma   comma(sep);
        for(const T& v : values){
            write(comma.text());
            write(v);
            ++comma;
        }
    }

    template <typename T, typename C>
    void    write(const Set<T,C>& values, const String& sep = ", ")
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
    void    write(const QStringList&, const String& sep=", ");
    void    write(const QUrl&);
    void    write(const String&);
    //void    write(const QVariant&);
    
    /*! \brief  Appends document to stream
    
        \param[in] documentname  Document to be appended
        \param[in] fSuppressMsg     Suppresses the log-message if TRUE
        
        \return TRUE if the document loaded successfull, FALSE otherwise
    */
    bool    append_document(const std::filesystem::path&documentname, bool fSuppressMsg=false);
    
    //! forces the doucment to "finish" for write
    virtual void    finish(){}
    
        //  Reference, must remain valid for duration of session
    void        setStream(Stream&);

protected:
    String      m_title;
    Stream*     m_stream;
    
    NetWriter();
    NetWriter(Stream&);
    ~NetWriter();

    
};

