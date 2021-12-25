////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Utilities.hpp"

#include <util/algo/DiffEngine.hpp>

#include <util/collection/List.hpp>
#include <util/collection/Set.hpp>
#include <util/collection/Vector.hpp>

#include <util/text/Comma.hpp>
#include <util/text/IgCase.hpp>
#include <util/text/String.hpp>

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
#include <QVariant>

namespace yq {

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    bool        any_changes(const Vector<DEResult>& results)
    {
        for(auto& de : results)
            if(de.mode != DEResult::Same)
                return true;
        return false;
    }

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

    bool isMainThread()
    {
        return is_main_thread();
    }

    bool    is_in(const String& a, const String& b)
    {
        if(a.size() != b.size())
            return false;
        for(size_t n=0;n<a.size();++n)
            if(tolower(a[n]) != tolower(b[n]))
                return false;
        return true;
    }

    bool    is_in(const String& a, const char* b)
    {
        if(!b)
            return false;
        size_t  n;
        for(n=0;n<a.size();++n){
            if(!b[n])
                return false;
            if(tolower(a[n]) != tolower(b[n]))
                return false;
        }
        return !b[n];
    }

    bool    is_in(const char* a, const String& b)
    {
        return is_in(b,a);
    }

    bool    is_in(const Vector<String>& vec, const String& b)
    {
        for(const String& a : vec)
            if(is_in(a,b))
                return true;
        return false;
    }

    bool    is_in(const std::initializer_list<String>& vec, const String& b)
    {
        for(const String& a : vec)
            if(is_in(a,b))
                return true;
        return false;
    }

    bool    is_in(const std::initializer_list<const char*>& vec, const String& b)
    {
        for(const char* a : vec)
            if(a && is_in(b,a))
                return true;
        return false;
    }


    String  join(const Set<uint16_t>& values, const String& sep)
    {
        Comma       c(sep);
        String      ret;
        for(uint16_t i : values){
            ret += c.text();
            ++c;
            ret += String::number(i);
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

    //QString     join(const QStringSet& them, const QString& sep)
    //{
        //return QStringList(qt(makeList(them))).join(sep);
    //}

    //QString     join(const QStringSet& them, QChar sep)
    //{
        //return QStringList(qt(makeList(them))).join(sep);
    //}

    String          join(const StringSet&them, const String&sep)
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

    //QString         join_qstring(const Set<uint16_t>& values, const QString& sep)
    //{
        //QString     ret;
        //bool        first = true;
        //for(uint16_t v  : values){
            //if(first)
                //first   = false;
            //else
                //ret += sep;
                
            //ret += QString::number(v);
        //}
        //return ret;
    //}

    //String          join_string(const Set<uint16_t>& values, const String& sep)
    //{
        //String      ret;
        //bool        first = true;
        //for(uint16_t v  : values){
            //if(first)
                //first   = false;
            //else
                //ret += sep;
                
            //ret += String::number(v);
        //}
        //return ret;
    //}



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

    StringSet       qt(const QStringSet& q)
    {
        StringSet   ret;
        for(const QString & s : q)
            ret << s;
        return ret;
    }

    QStringSet      qt(const StringSet&q)
    {
        QStringSet  ret;
        for(const String& s : q)
            ret << s.qString();
        return ret;
    }

    Vector<String>  qt(const Vector<QString>&q)
    {
        Vector<String> ret;
        for(const QString& s : q)
            ret << s;
        return ret;
    }

    Vector<QString> qt(const Vector<String>&q)
    {
        Vector<QString> ret;
        for(const String& s : q)
            ret << s.qString();
        return ret;
    }

    QStringList     qt_list(const StringSet&q)
    {
        QStringList     ret;
        for(const String& s: q)
            ret << s.qString();
        return ret;
    }

    QStringList     qt_list(const QStringSet&q)
    {
        QStringList     ret;
        for(const QString& s: q)
            ret << s;
        return ret;
    }

    QVariantList    qvar_list(const StringSet& slist)
    {
        QVariantList    ret;
        for(const String& s : slist)
            ret << s.qString();
        return ret;
    }

    QVariantList    qvar_list(const QStringSet& slist)
    {
        QVariantList    ret;
        for(const QString& s : slist)
            ret << s;
        return ret;
    }


    QString         sanitize_path(const QString& path)
    {
        return sanitize_path(path.split('/')).join('/');
    }

    String          sanitize_path(const String& path)
    {
        return sanitize_path(path.qString());
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

    String          strip_extension(const String& name)
    {
        return strip_extension(name.qString());
    }

    QIcon           theme_icon(const QString& icon)
    {
        return theme_icon(icon, QString());
    }

    QIcon           theme_icon(const QString& icon, const QString& fallback)
    {
        if(is_main_thread()){
            QIcon   ico     = QIcon::fromTheme(icon);
            if(!ico.isNull())
                return ico;
            if(!fallback.isEmpty()){
                if(fallback.contains('%'))
                    ico     = fetch_icon(fallback);
                else
                    ico     = QIcon(fallback);
            }
            return ico;
        } else {
            return QIcon();
        }
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

}
