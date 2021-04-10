////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QApplication>
#include <memory>
#include <util/Map.hpp>
#include <QVariant>

class QUrl;

/*! \brief Application for Quill editors

    This is the QApplication for common init for Quill editors
*/
class QuillApp : public QApplication {
    Q_OBJECT
public:

    static constexpr const char*  szAutoLoadLast          = "bAutoLoadLast";
    static constexpr const char*  szRecent                = "slRecent";
    static constexpr const char*  szMaxRecent             = "iMaxRecent";
    static constexpr const char*  szLastWindowPosition    = "rLastWindowPos";
    static constexpr const char*  szAutoBackup            = "iAutoBackup";
    static constexpr const char*  szThumbnailWidth        = "iThumbnailWidth";
    static constexpr const char*  szThumbnailHeight       = "iThumbnailHeight";
    static constexpr const char*  szUndoStackLimit        = "iUndoStackLimit";

    /*! \brief Creates the app
    */
    QuillApp(int&, char**);
    ~QuillApp();
    
    /*! \brief Initializes the *best* workspace from the specified path. 
    
        This is for local initialization which will include launching the database-scanner (if not 
        yet running).
    
        \note Any settings-lookupas/prompts are the responsibility of the caller
        \param[path]    Specified path
    */
    bool                    init_local_workspace(const QString& path);
    
    static QuillApp*        app() { return s_app; }
    static QString          yq_exe(const QString&);
    static ushort           port();

    static bool             is_valid(const QUrl&);
    static QUrl             url(const QString&path);
    static QUrl             url(const QString&path, const QString& query);
    static QUrl             url(const QString&path, const Map<QString,QVariant>& query);
    
    static const QString&   url_base();
    
    template <typename T>
    static T                get_setting(const QString& k, const T& def={})
    {
        auto i = get_setting_impl(k);
        if(i.second)
            return i.first.value<T>();
        return def;
    }
    
    template <typename T>
    static void             set_setting(const QString&k, const T& val, bool fGlobal=false)
    {
        set_setting_impl(k, QVariant::fromValue(val), fGlobal);
    }
    
    static unsigned int     auto_backup();
    static void             set_auto_backup(unsigned int);
    static bool             auto_load_last();
    static void             set_auto_load_last(bool);
    static unsigned int     thumbnail_height();
    static void             set_thumbnail_height(unsigned int);
    static unsigned int     thumbnail_width();
    static void             set_thumbnail_width(unsigned int);
    static unsigned int     undo_stack_limit();
    static bool             remember_recent();
    static unsigned int     max_recent();
    static QStringList      recent();
    static void             add_recent(const QString&);
    static QRect            last_window_pos();
    static void             set_last_window_pos(const QRect&);
    
private:

    static std::pair<QVariant,bool> get_setting_impl(const QString&);
    static void                     set_setting_impl(const QString&, const QVariant&, bool fGlobal=false);


    struct Impl;
    static Impl&            impl();
    static QuillApp*        s_app;
    Impl&                   m;
};
