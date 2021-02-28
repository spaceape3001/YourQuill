////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "QuillApp.hpp"

#include <db/QuillFile.hpp>
#include <db/Workspace.hpp>
#include <meta/Meta.hpp>
#include <util/Curl.hpp>
#include <util/Execute.hpp>
#include <util/Logging.hpp>

#include <unistd.h>
#include <curl/curl.h>
#include <tbb/spin_rw_mutex.h>

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QProcess>
#include <QRect>
#include <QSettings>
#include <QStringList>
#include <QTextCodec>
#include <QUrl>
#include <QUrlQuery>

void    init_resources();

namespace {
    char**  pre_init(char** z)
    {
        log_to_std_error();
        curl_global_init(CURL_GLOBAL_ALL);
        init_resources();
        return z;
    }

    static bool         defAutoLoadLast         = false;
    static unsigned int defMaxRecent            = 10;
    static unsigned int defAutoBackup           = 9;
    static unsigned int defThumbnailWidth       = 96;
    static unsigned int defThumbnailHeight      = 64;
    static unsigned int defUndoStackLimit       = 0;
}

/*
    Quick thought... move to a PID directory.... this would allow for processes that need the database?
    Have the database monitor that PID if launched via GUIs (maybe even a catch on run)?
    A signal/command? -- web page!  (Post, of course)
*/

struct QuillApp::Impl {
    QString             appName;
    QDir                appDir;
    QString             appDirPath;
    uint16_t            port;
    QString             scheme, host, url;
    QProcess*           db_proc;
    QSettings*          settings;
    tbb::spin_rw_mutex  mutex;
    
    Impl() : port{}, db_proc{}, settings{} {}
};

    #define LOCK                                                    \
        auto& _i = impl();                                           \
        tbb::spin_rw_mutex::scoped_lock _lock(_i.mutex, false);

    #define WLOCK                                                   \
        auto& _i = impl();                                           \
        tbb::spin_rw_mutex::scoped_lock _lock(_i.mutex, true);

QuillApp*        QuillApp::s_app = nullptr;

QuillApp::Impl&            QuillApp::impl()
{
    static Impl    i;
    return i;
}

QString   QuillApp::yq_exe(const QString&s)
{
    return impl().appDir.absoluteFilePath(s);
}

std::pair<QVariant,bool> QuillApp::get_setting_impl(const QString&k)
{
    QString     sk  = QString("%1/%2").arg(impl().appName).arg(k);
    
    LOCK
    if(!_i.settings)
        return std::pair<QVariant,bool>(QVariant(), false);
    if(_i.settings->contains(sk))
        return std::pair<QVariant,bool>(_i.settings->value(sk),true);
    if(_i.settings->contains(k))
        return std::pair<QVariant,bool>(_i.settings->value(k),true);
    return std::pair<QVariant,bool>(QVariant(), false);
}

void             QuillApp::set_setting_impl(const QString&k, const QVariant&v, bool fGlobal)
{
    QString     sk  = fGlobal ? k : QString("%1/%2").arg(impl().appName).arg(k);
    WLOCK
    if(!_i.settings)    
        return;
    _i.settings -> setValue(k, v);
}

bool             QuillApp::is_valid(const QUrl& url)
{
    const Impl& i   = impl();
    if(url.scheme() != i.scheme)
        return false;
    if(url.host() != i.host)
        return false;
    if(url.port() != i.port)
        return false;
    return true;
}


QUrl             QuillApp::url(const QString&path)
{
    QUrl    u(impl().url);
    u.setPath(path);
    return u;
}

QUrl             QuillApp::url(const QString&path, const QString& query)
{
    QUrl    u(impl().url);
    u.setPath(path);
    u.setQuery(query);
    return u;
}

QUrl             QuillApp::url(const QString&path, const Map<QString,QVariant>& args)
{
    QUrl    u = url(path);
    QUrlQuery   uq;
    for(auto& i : args)
        uq.addQueryItem(i.first, i.second.toString());
    u.setQuery(uq);
    return u;
}

const QString&   QuillApp::url_base()
{
    return impl().url;
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void             QuillApp::add_recent(const QString& s)
{
    unsigned int    mx  = max_recent();
    WLOCK
    if(!_i.settings)
        return ;

    QString         sk  = QString("%1/%2").arg(_i.appName).arg(szRecent);
    if(!mx){
        _i.settings -> remove(sk);
        return ;
    }

    QStringList     sl  = _i.settings->value(sk).value<QStringList>();
    sl.removeAll(s);
    sl.prepend(s);
    if(sl.size() > (int) mx)
        sl.erase(sl.begin() + mx, sl.end());
    _i.settings->setValue(sk, sl);
}

unsigned int     QuillApp::auto_backup()
{
    return get_setting(szAutoBackup, defAutoBackup);
}

bool             QuillApp::auto_load_last()
{
    return get_setting(szAutoLoadLast, defAutoLoadLast);
}

QRect            QuillApp::last_window_pos()
{
    return get_setting<QRect>(szLastWindowPosition);
}

unsigned int     QuillApp::max_recent()
{
    return get_setting(szMaxRecent, defMaxRecent);
}

ushort           QuillApp::port()
{
    return impl().port;
}

QStringList      QuillApp::recent()
{
    return get_setting(szRecent, QStringList());
}

bool             QuillApp::remember_recent()
{
    return max_recent() > 0;
}

void             QuillApp::set_auto_backup(unsigned int v)
{
    set_setting(szAutoBackup, v, true);
}

void             QuillApp::set_auto_load_last(bool v)
{
    set_setting(szAutoLoadLast, v, false);
}

void             QuillApp::set_last_window_pos(const QRect&v)
{
    set_setting(szLastWindowPosition, v, false);
}

void             QuillApp::set_thumbnail_height(unsigned int v)
{
    set_setting(szThumbnailHeight, v, true);
}

void             QuillApp::set_thumbnail_width(unsigned int v)
{
    set_setting(szThumbnailWidth, v, true);
}


unsigned int     QuillApp::thumbnail_height()
{
    return get_setting(szThumbnailHeight, defThumbnailHeight);
}

unsigned int     QuillApp::thumbnail_width()
{
    return get_setting(szThumbnailWidth, defThumbnailWidth);
}

unsigned int     QuillApp::undo_stack_limit()
{
    return get_setting(szUndoStackLimit, defUndoStackLimit);
}



//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

QuillApp::QuillApp(int&argc, char**argv) : QApplication(argc, pre_init(argv)), m(impl())
{
    setOrganizationName("NakedPyro");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    meta_init();
    s_app           = this;
    m.appName       = applicationName();
    m.appDirPath    = applicationDirPath();
    m.appDir        = QDir(m.appDirPath);
    m.settings      = new QSettings("NakedPyro", "Quill", this);
}

QuillApp::~QuillApp()
{
    s_app       = nullptr;
}

bool      QuillApp::init_local_workspace(const QString& path)
{
    if(path.isEmpty()){
        yError() << "Unable to initialize workspace to an empty string!\n";
        return false;
    }
    
    //  resolve to a quill file
    QString         qFile   = wksp::best_guess(path);
    if(qFile.isEmpty()){
        yError() << "Unable to resolve " << path << " to a quill workspace.";
        return false;
    }

    //  pre-init the workspace
    QuillFile        q;
    if(!q.load(qFile)){
        yError() << "Unable to load " << qFile;
        return false;
    }

    Set<uint16_t>   ports;
    if(q.port)
        ports << q.port;
    ports += q.aux_ports;
    
    bool    running = false;
    for(uint16_t p : ports){
        Curl        c;
        QString     h   = tr("http://localhost:%1").arg(p);
        c.set_url(QString("%1/api/wksp/quill").arg(h));
        if(c.exec() != HttpStatus::Success)
            continue;
        QJsonDocument   jd  = c.rx_json();
        if(jd.isNull())
            continue;
        const QJsonObject   jo  = jd.object();
        QString s   = jo["quill"].toString();
        if(s.isEmpty())
            continue;
        QFileInfo   fi(s);
        if(fi.exists() && fi.isDir()){
            yInfo() << "Database already running on port " << p << ", using it.";
            m.port      = p;
            running     = true;
            qFile       = s;
            m.url       = h;
        }
    }
    
    if(!wksp::initialize(qFile, wksp::Options()))
        return false;
    
    //  Test ....
    
    m.scheme    = "http";   // right now, kinda hardcoded
    m.host      = "localhost";
    
    if(!running){
    
        //  start the scanner....
        //  FOR NOW, we're not detaching this.... eventually we'll smarten DB up.
        m.db_proc   = new QProcess(this);
        m.db_proc -> setProgram(yq_exe("db"));
        m.db_proc -> setArguments(QStringList() << qFile);
        m.db_proc -> start();
        
        //  TODO make the DB scanner smarter to be detached and auto-kill when users are no-longer present
        //  (ie, a temporary file in a "client" subdirectory?
        
        if(q.port){
            m.port      = q.port;
            m.url       = tr("http://localhost:%1").arg(q.port);
        }
        usleep(100);
    }
    
    //  TODO
    
    return true;
}



#include "moc_QuillApp.cpp"
