#include "Utilities.hpp"

#include "Comma.hpp"
#include "List.hpp"
#include "Set.hpp"
#include "String.hpp"
#include "Vector.hpp"

#include <iostream>
#include <sys/random.h>

#include <QByteArray>
#include <QBuffer>
#include <QCoreApplication>
#include <QFileInfo>
#include <QGuiApplication>
#include <QIcon>
#include <QImage>
#include <QScreen>
#include <QThread>


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

uint64_t        randU64()
{
    static constexpr const  uint64_t    SZ      = 32;
    static thread_local     uint64_t    buffer[SZ];
    static thread_local     uint64_t    nxt     = SZ;
    
    if(nxt >= SZ){
        (void) !getrandom(buffer, sizeof(buffer), 0);
        nxt     = 0;
    }
    
    return buffer[nxt++];
}


QRect       desktop_screen_box()
{
    QRect   r;
    for(QScreen* sc : QGuiApplication::screens()){
        if(r.isEmpty())
            r   = sc -> geometry();
        else
            r |= sc -> geometry();
    }
    return r;
}

QIcon       fetch_icon(const QString&file)
{
    if(is_main_thread()){
        static const char *sizes[] = { "16", "016", "24", "024", "32", "032", "48",
                                       "048", "64", "064", "96", "096", "128", 
                                       "144", "192", "256" };
        QIcon   res;
        for(const char* z : sizes){
            QString name    = file.arg(z);
            if(QFileInfo(name).exists())
                res.addFile(name);
        }
        
        return res;
    } else
        return QIcon();
}


bool is_main_thread()
{
    return QThread::currentThread() == QCoreApplication::instance()->thread();
}

QString  join(const Set<uint16_t>& values, const QString& sep)
{
    Comma       c(sep);
    QString     ret;
    for(uint16_t i : values){
        ret += c.text();
        ++c;
        ret += QString::number(i);
    }
    return ret;
}



QByteArray     join(const Vector<QByteArray>&them, char sep)
{
    bool        first = true;
    QByteArray  ret;
    for(const QByteArray& t : them){
        if(first){
            first   = false;
        } else {
            ret += sep;
        }
        ret += t;
    }
    return ret;
}

QByteArray     join(const Vector<QByteArray>&them, const QByteArray& sep)
{
    bool        first = true;
    QByteArray  ret;
    for(const QByteArray& t : them){
        if(first){
            first   = false;
        } else {
            ret += sep;
        }
        ret += t;
    }
    return ret;
}

QString     join(const Vector<QString>&them, char sep)
{
    bool        first = true;
    QString  ret;
    for(const QString& t : them){
        if(first){
            first   = false;
        } else {
            ret += sep;
        }
        ret += t;
    }
    return ret;
}

QString     join(const Vector<QString>&them, const QString& sep)
{
    bool        first = true;
    QString  ret;
    for(const QString& t : them){
        if(first){
            first   = false;
        } else {
            ret += sep;
        }
        ret += t;
    }
    return ret;
}

String      join(const Vector<String>&them, char sep)
{
    bool        first = true;
    String  ret;
    for(const String& t : them){
        if(first){
            first   = false;
        } else {
            ret += sep;
        }
        ret += t;
    }
    return ret;
}

String      join(const Vector<String>&them, const String& sep)
{
    bool        first = true;
    String  ret;
    for(const String& t : them){
        if(first){
            first   = false;
        } else {
            ret += sep;
        }
        ret += t;
    }
    return ret;
}

QString     join(const QStringSet& them, const QString& sep)
{
    return QStringList(qt(makeList(them))).join(sep);
}

QString     join(const QStringSet& them, QChar sep)
{
    return QStringList(qt(makeList(them))).join(sep);
}

QByteArray  make_png(const QImage&img)
{
    if(img.isNull())
        return QByteArray();

    QBuffer     buf;
    buf.open(QIODevice::WriteOnly);
    img.save(&buf, "PNG");
    buf.close();
    return buf.buffer();
}

QString         sanitize_path(const QString& path)
{
    return sanitize_path(path.split('/')).join('/');
}

QStringList     sanitize_path(const QStringList& path)
{
    QStringList ret;
    for(const QString& s : path){
        if(s.isEmpty())
            continue;
        if(s == ".")
            continue;
        if(s == "..")
            continue;
        ret << s;
    }
    if(path.last().isEmpty())   // preserve the final empty for a "/"
        ret << QString();
    return ret;
}

QString         strip_extension(const QString& name)
{
    int     x   = name.lastIndexOf('.');
    int     s   = name.lastIndexOf('/');
    if(x > s)
        return name.mid(0,x);
    return name;
}


QString         utf8(const QByteArray&b)
{
    return QString::fromUtf8(b);
}

QByteArray      utf8(const QString&b)
{
    return b.toUtf8();
}

std::ostream& operator<<(std::ostream&os, const QString& v)
{
    return os << v.toStdString();
}
