////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <yq/preamble.hpp>

class QDir;
class QFileInfo;
class QString;

namespace yq {


    using qdir_r        = Result<QDir>;
    using qfileinfo_r   = Result<QFileInfo>;

    //! Directory of the build
    //const char*                 build_dir_path();


    //! Vector of the paths for share directories
    //!
    //! \note these are in the order of desired search (in case of conflicts)
    const Vector<QString>&      share_dir_paths();

    //! Vector of QDirs for the share directories
    const Vector<QDir>&         share_dirs();

    QByteArray                  shared_bytes(const char*);

    //! Resolves the name to a directory (returns nothing if not exists)
    qdir_r                      shared_dir(const QString&);

    Vector<QDir>                shared_dirs(const QString&);

    //! Resolves the name to a file (returns nothing if not exists)
    qstring_r                   shared_file(const QString&);

    //! Gets all matches
    Vector<QString>             shared_files(const QString&);

    qfileinfo_r                 shared_info(const QString&);

    //! Gets infos for all
    Vector<QFileInfo>           sharedInfos(const QString&);

    //! Resolved the name to a path (file/dir) and returns nothing if not exists
    qstring_r                   shared_path(const QString&);

    Vector<QString>             shared_paths(const QString&);
}
