////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ShareDir.hpp"

#include <util/FileUtils.hpp>
#include <util/Result.hpp>
#include <util/Vector.hpp>

#include <QDir>
#include <QFile>
#include <QString>


const char*                build_dir_path()
{
    return YQ_BUILD_ROOT;
}

namespace {
    Vector<QString>         _shareDirPaths()
    {
        return Vector<QString>() << QDir::home().filePath(".yquill") << YQ_SHARE_DIR;   // TODO Smart search
    }
}

const Vector<QString>&      share_dir_paths()
{
    static Vector<QString>  s_ret   = _shareDirPaths();
    return s_ret;
}


namespace {
    Vector<QDir>            _shareDirs()
    {
        Vector<QDir>    ret;
        for(QString s : share_dir_paths())
            ret << QDir(s);
        return ret;
    }
}

const Vector<QDir>&         share_dirs()
{
    static Vector<QDir> s_ret   = _shareDirs();
    return s_ret;
}

QByteArray                  shared_bytes(const char*z)
{
    Vector<uint8_t> sp = file_load_all(shared_file(z));
    return QByteArray((const char*) sp.data(), sp.size());
}



//! Resolves the name to a directory (returns nothing if not exists)
qdir_r                  shared_dir(const QString& z)
{
    for(const QDir& d : share_dirs()){
        QString     c   = d.absoluteFilePath(z);
        QFileInfo   fi(c);
        if(fi.exists() && fi.isDir())
            return QDir(c);
    }
    return qdir_r();
}

Vector<QDir>            shared_dirs(const QString&z)
{
    Vector<QDir>    ret;
    for(const QDir& d : share_dirs()){
        QString     c   = d.absoluteFilePath(z);
        QFileInfo   fi(c);
        if(fi.exists() && fi.isDir())
            ret << QDir(c);
    }
    return ret;
}


//! Resolves the name to a file (returns nothing if not exists)
qstring_r               shared_file(const QString& z)
{
    for(const QDir& d : share_dirs()){
        QString     c   = d.absoluteFilePath(z);
        QFileInfo   fi(c);
        if(fi.exists() && fi.isFile())
            return c;
    }
    return qstring_r();
}

Vector<QString>         shared_files(const QString&z)
{
    Vector<QString> ret;
    for(const QDir& d : share_dirs()){
        QString     c   = d.absoluteFilePath(z);
        QFileInfo   fi(c);
        if(fi.exists() && fi.isFile())
            return ret << c;
    }
    return ret;
}


qfileinfo_r             shared_info(const QString&z)
{
    for(const QDir& d : share_dirs()){
        QString     c   = d.absoluteFilePath(z);
        QFileInfo   fi(c);
        if(fi.exists())
            return fi;
    }
    return qfileinfo_r();
}

Vector<QFileInfo>       shared_infos(const QString&z)
{
    Vector<QFileInfo>   ret;
    for(const QDir& d : share_dirs()){
        QString     c   = d.absoluteFilePath(z);
        QFileInfo   fi(c);
        if(fi.exists())
            return ret << fi;
    }
    return ret;
}


//! Resolved the name to a path (file/dir) and returns nothing if not exists
qstring_r       shared_path(const QString& z)
{
    for(const QDir& d : share_dirs()){
        QString     c   = d.absoluteFilePath(z);
        QFileInfo   fi(c);
        if(fi.exists())
            return c;
    }
    return qstring_r();
}

Vector<QString>             shared_paths(const QString&z)
{
    Vector<QString> ret;
    for(const QDir& d : share_dirs()){
        QString     c   = d.absoluteFilePath(z);
        QFileInfo   fi(c);
        if(fi.exists())
            ret << c;
    }
    return ret;
}
