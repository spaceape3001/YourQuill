////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Root.hpp"
#include "QuillFile.hpp"
#include "Workspace.hpp"

#include "yq/Loggers.hpp"
#include "yq/bit/Copyright.hpp"
#include "yq/io/ShareDir.hpp"
#include "yq/meta/Global.hpp"
#include "yq/meta/GlobalImpl.hpp"
#include "yq/util/DelayInit.hpp"
#include "yq/util/Utilities.hpp"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTextCodec>

#include <unistd.h>
#include <tbb/spin_rw_mutex.h>

namespace {
    static constexpr const unsigned short       kDefaultPort        = 25505;
    static constexpr const unsigned int         kDefaultReadTimeout = 30000;

    struct M {
        QString             abbreviation;
        QString             app_name;
        QString             author;
        Set<uint16_t>       aux_ports;
        QString             db_pid;
        QString             cache;
        Copyright           copyright;
        QString             dot;
        QString             git;
        QString             home;
        QString             hostname;
        QString             ini;
        bool                init;
        QString             local_user;
        QDir                log_dir;
        QString             log_fragment;
        QString             logs;
        QString             markdown;
        QString             name;
        QString             perl;
        unsigned short      port;
        QString             quill_arg;
        QDir                quill_dir;
        QString             quill_dir_path;
        QString             quill_full_path;
        QString             quill_key;
        QString             quill_prime;
        unsigned int        read_timeout;
        RootVector          roots;
        RootMap             root_by_path;
        RoleMap             root_first;
        RoleVecMap          root_read;
        RoleVecMap          root_write;
        QString             smartypants;
        QString             start;
        QDateTime           start_time;
        QString             subversion;
        
        QDirVector          template_dirs;
        QStringSet          templates;
        QStringSet          templates_avail;
        
        QString             temporary;
        QDir                temporary_dir;
        
        mutable tbb::spin_rw_mutex  mutex;

        M() : init(false), port(0), read_timeout(0) {}
    };
    
    M&  impl()
    {
        static M s_ret;
        return s_ret;
    }

    struct SetSpots {
        unsigned int    abbr;
        unsigned int    author;
        unsigned int    copyfrom;
        unsigned int    copystance;
        unsigned int    copytext;
        unsigned int    copyto;
        unsigned int    home;
        unsigned int    name;
        
        SetSpots() : abbr(~0), author(~0), copyfrom(~0), 
                    copystance(~0), copytext(~0), copyto(~0), 
                    home(~0), name(~0) {}
    };

    QString     dirPathFor(const std::filesystem::path& pth)
    {
        return QFileInfo( QString::fromStdString(pth.string())).absolutePath();
    }
    

    Vector<Root*>       _roots()
    {
        Vector<Root*>  ret;
        for(const Root* r : impl().roots)
            ret << const_cast<Root*>(r);
        return ret;
    }
    
    QSqlDatabase        connectDB()
    {
        QSqlDatabase    db      = QSqlDatabase::addDatabase("QSQLITE", QString("CacheDB %1").arg(thread_id()));
        db.setDatabaseName(impl().cache);
        if(!db.open()){
            yError() << "Cannot open the database: " <<  db.lastError();
            return QSqlDatabase();
        }
        yInfo() << "Database connection " << impl().cache;
        return db;
    }
    
    #define LOCK                                                    \
        const M& m = impl();                                        \
        tbb::spin_rw_mutex::scoped_lock _lock(m.mutex, false);
    #define WLOCK                                                   \
        M& m = impl();                                              \
        tbb::spin_rw_mutex::scoped_lock _lock(m.mutex, true);
}


namespace wksp {
    struct Init {   // Only a class to support "friend"
        static void rootList(const QString& pfx=QString())
        {
            const M&  m = impl();
            yInfo () << pfx << m.roots.size() << " roots";
            for(const Root* rt : m.roots)
                yInfo () << pfx << " " << rt->path();
        }
    
    
        static bool _init(const QString& location, Options opts)
        {
            bool    ret     = true;
            M&  m = impl();
            tbb::spin_rw_mutex::scoped_lock    _lock(m.mutex, true);
            
            if(m.init){
                wkspError << "Workspace is already initialized!";
                return false;
            }
            
            if(thread_id() != 0){
                wkspError << "Workspace *MUST* be initialized from the main thread!";
                return false;
            }

            //yInfo() << "wksp::Init::_init('" << location << "')";
            
            m.start_time    = QDateTime::currentDateTimeUtc();
            m.start         = m.start_time.toString("yyyyMMdd-HHmmss");
            m.app_name      = QCoreApplication::applicationName();
            
            char        hname[HOST_NAME_MAX+1];
            gethostname(hname, sizeof(hname));
            hname[HOST_NAME_MAX]    = '\0';
            m.hostname      = QString(hname);
            
            //  Right now, hardcode these
            m.dot           = "/usr/bin/dot";
            m.git           = "/usr/bin/git";
            m.perl          = "/usr/bin/perl";
            m.subversion    = "/usr/bin/svn";
            
            m.markdown      = shared_file("perl/Markdown.pl").value;
            m.smartypants   = shared_file("perl/SmartyPants.pl").value;
            
            QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
            
            m.template_dirs = shared_dirs("template");
            for(const QDir& d : m.template_dirs)
                for(QString s : d.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
                    m.templates_avail << s;
            
            m.quill_arg         = location;
            if(opts.is_set(Option::SEARCH)){
                m.quill_prime   = best_guess(location);
            } else {
                QFileInfo   fi(location);
                if(fi.fileName() == szQuillFile){
                    m.quill_prime   = location;
                } else if(fi.isDir()){
                    m.quill_prime    = QDir(location).absoluteFilePath(szQuillFile);
                }
            }
            
            if(m.quill_prime.isEmpty()){
                wkspError << "No " << szQuillFile << " fragment at specified location, REQUIRED!";
                return false;
            }
            
            QuillFile        doc;
            if(!doc.load(m.quill_prime.toStdString())){
                wkspError << "Unable to load the Quill fragment.";
                return false;
            }
            
            m.quill_dir_path        = dirPathFor(doc.file());
            m.quill_dir             = QDir(m.quill_dir_path);
            m.quill_key             = QFileInfo(m.quill_dir_path).fileName();
            
            String      temp    = doc.temp_dir();
            if(temp.empty()){
                const char* tmpdir  = getenv("TMPDIR");
                if(!tmpdir)
                    tmpdir          = "/tmp";
                QDir    yqtemp      = QDir(tmpdir).absoluteFilePath("yquill");
                m.temporary         = yqtemp.absoluteFilePath(m.quill_key);
            } else
                m.temporary         = m.quill_dir.absoluteFilePath(temp.qString());
            m.quill_dir.mkpath(m.temporary);
            m.temporary_dir         = QDir(m.temporary);
            
            String      cache   = doc.cache();
            if(cache.empty()){
                m.cache             = m.temporary_dir.absoluteFilePath("cache");
            } else
                m.cache             = m.quill_dir.absoluteFilePath(cache.qString());
            m.db_pid                = m.temporary_dir.absoluteFilePath("pid");
            
            //if(opts.is_set(Option::WIPE_CACHE) && QFile::exists(m.cache) )
                //QFile::remove(m.cache);

            String logdir           = doc.log_dir();
            
            if(logdir.empty()){
                m.logs              = m.temporary_dir.absoluteFilePath("logs");
            } else
                m.logs              = m.quill_dir.absoluteFilePath(logdir.qString());
            m.quill_dir.mkpath(m.logs);
            m.log_dir               = QDir(m.logs);
            m.log_fragment          = m.log_dir.absoluteFilePath(QString("%1-%2.log").arg(m.app_name).arg(m.start));

            log_to_file(m.log_fragment.toStdString(), LogPriority::YQ_DBGREL(All,All));
            yInfo() << "Application '" << m.app_name << "' is initializing the workspace to '" << m.quill_dir_path << "'";

            String  ini = doc.ini();
            if(ini.empty()){
                m.ini               = m.temporary_dir.absoluteFilePath("ini");
            } else
                m.ini               = m.quill_dir.absoluteFilePath(ini.qString());
            
            String luser            = doc.local_user();
            if(luser.empty()){
                m.local_user        = getlogin();
            } else 
                m.local_user        = luser.qString();
                
            m.aux_ports             = doc.aux_ports();
            m.port                  = doc.port();
            if(!m.port)
                m.port              = kDefaultPort;
            m.read_timeout          = doc.read_timeout();
            if(!m.read_timeout)
                m.read_timeout      = kDefaultReadTimeout;

            SetSpots            spots;
            m.abbreviation          = doc.abbreviation().qString();
            if(!m.abbreviation.isEmpty())
                spots.abbr          = 0;
            m.author                = doc.author().qString();
            if(!m.author.isEmpty())
                spots.author        = 0;
            m.copyright             = doc.copyright();
            if(!m.copyright.from)
                spots.copyfrom      = 0;
            if(!m.copyright.stance)
                spots.copystance    = 0;
            if(!m.copyright.text.empty())
                spots.copytext      = 0;
            if(!m.copyright.to)
                spots.copyto        = 0;
            m.home                  = doc.home().qString();
            if(!m.home.isEmpty())
                spots.home          = 0;
            m.name                  = doc.name().qString();
            if(!m.name.isEmpty())
                spots.name          = 0;
            
            PathSet                 rSeen, tSeen;
            
            //  The first root (the quill)
            Root*               rt           = new Root(m.quill_dir_path, PolicyMap(Access::ReadWrite));
            rt -> m_id                       = 0;
            rt -> m_depth                    = 0;
            m.roots << rt;
            m.root_by_path["."]              = rt;
            m.root_by_path[m.quill_dir_path] = rt;

            _load(doc, rSeen, tSeen, PolicyMap(Access::ReadWrite), 
                PolicyMap(opts.is_set(Option::RO_TEMPLATE) ? Access::ReadOnly : Access::ReadWrite), spots, false, 1);
            
            for(const auto &i : tSeen)
                m.templates << i;
            
            m.roots.sort([](const Root* a, const Root* b){
                if(a->is_template() != b->is_template())
                    return a->is_template() < b->is_template();
                if(a->depth() != b->depth())
                    return a->depth() < b->depth();
                return a->id() < b->id();
            });
            
            Vector<Root*>  eroots  = _roots();
            for(Root* r2 : eroots){
                QDir().mkpath(r2->resolve(szConfigDir));
            }
                
            StringSet   keys;
            
            uint64_t        i   = 0;
            for(Root* r2 : eroots){
                r2 -> m_id      = i++;      // Re-ID everybody with the new order
                if(!Root::is_good_dir(r2->path(), Root::G::Writable)){
                    for(DataRole dr : DataRole::all_values())
                        r2 -> m_roles[dr].access    = moderate(r2 -> m_roles[dr].access, Access::ReadOnly);
                }
                keys << r2 -> key();
            }
            
                //  make sure everything has a key, default is the number
            for(Root* r2 : eroots){
                if(!r2 -> m_key.isEmpty())
                    continue;
                String s   = String::number(r2->id());
                if(!keys.has(s)){
                    r2 -> m_key = s.qString();
                    keys << s;
                }
                s += '_';
                for(i=0;; ++i){
                    String  k   = s + String::number(i);
                    if(!keys.has(k)){
                        r2 -> m_key = k.qString();
                        keys << k;
                    }
                }
            }
            
            //  Sort out the writers
            for(DataRole dr : DataRole::all_values()){
                bool    hasFirst    = false;
                for(Root* r3 : eroots){
                    if(r3->m_roles[dr].access == Access::WriteFirst){
                        if(hasFirst)    
                            r3->m_roles[dr].access   = Access::ReadWrite;
                        else
                            hasFirst    = true;
                    }
                }
                
                if(!hasFirst){
                    for(Root* r3 : eroots){
                        if(r3->m_roles[dr].access == Access::WriteFirst) {
                            r3 -> m_roles[dr].access    = Access::WriteFirst;
                            hasFirst    = true;
                            break;
                        }
                    }
                }
                
                if(!hasFirst)
                    m.root_first[dr]     = nullptr;
                
                //  now the read/write order
                
                uint64_t        ro  = 0;
                uint64_t        wo  = 0;
                
                for(Root* r3 : eroots){
                    switch(r3->m_roles[dr].access){
                    case Access::WriteFirst:
                        m.root_first[dr]     = r3;
                        /* fall through */
                    case Access::ReadWrite:
                        m.root_write[dr] << r3;
                        r3->m_roles[dr].writeOrder  = wo++;
                        /* fall through */
                    case Access::ReadOnly:
                        m.root_read[dr] << r3;
                        r3->m_roles[dr].readOrder   = ro++;
                        /* fall through */
                    case Access::NoAccess:
                    default:
                        break;
                    }
                }
            }
            
            
            m.init      = true;
            return ret;
        }

        static bool       _load(const QuillFile&doc, PathSet&rSeen, PathSet&tSeen, PolicyMap rPolicy, PolicyMap tPolicy, SetSpots&spots, bool fTemplate, size_t depth)
        {
            M& m = impl();
            QDir        q(dirPathFor(doc.file()));
            for(auto& rs : doc.templates()){
                QString rsPath  = rs.path.qString();
                if(!tSeen.add(rsPath))
                    continue;
                PolicyMap   pm2 = moderate(tPolicy, rs.policy);
                for(QString t : template_dir_paths(rsPath)){
                    Root*   rt          = nullptr;
                    bool    rcre        = false;
                    std::tie(rt,rcre)   = _root(t);
                    if(rcre)

                        rt -> m_depth   = depth;
                    for(DataRole dr : DataRole::all_values()){
                        if(pm2[dr] == Access::Default)
                            pm2[dr] = tPolicy[dr];
                        rt -> m_roles[dr].access    = pm2[dr];
                    }
                    rt->m_name          =  ":" + rsPath;
                    rt->m_key           = rs.key.qString();
                    if(rt->m_key.isEmpty())
                        rt->m_key       = rt->m_name;
                    rt->m_isTemplate    = true;
                    rt->m_color         = rs.color.qString();
                
                    QString     qf  = quill_fragment_for_dir(t);
                    if(!QFile::exists(qf)){
                        yDebug() << "Quill fragment " << qf << " does not exist (not required to).";
                        continue;
                    }
                    
                    QuillFile        sub;
                    if(!sub.load(qf.toStdString())){
                        yWarning() << "Unable to read Quill fragment: " << qf;
                        continue;
                    }
                    
                    _load(sub, rSeen, tSeen, rPolicy, pm2, spots, true, depth+1);
                }
            }
            
            if(!fTemplate){
                for(auto& rs : doc.roots()){
                    QString     rsPath  = rs.path.qString();
                    QString     rpath   = q.absoluteFilePath(rsPath);
                    if(rsPath == ".")
                        rpath          = q.absolutePath();

                    if(!rSeen.add(rpath))   // already seen
                        continue;
                        
                    PolicyMap   pm2 = moderate(rPolicy, rs.policy);    
                    
                    Root*   rt      = nullptr;
                    bool    rcre    = false;
                    std::tie(rt,rcre)   = _root(rpath);
                    if(rcre)
                        rt -> m_depth   = depth;
                        
                    for(DataRole dr : DataRole::all_values()){
                        if(pm2[dr] == Access::Default)
                            pm2[dr] = rPolicy[dr];
                        rt -> m_roles[dr].access    = pm2[dr];
                    }
                    rt -> m_name        = rs.name.qString();
                    rt -> m_key         = rs.key.qString();
                    if(rt -> m_key.isEmpty())
                        rt -> m_key     = rt -> m_name;
                    if(rs.vcs != Vcs())
                        rt -> m_vcs    |= rs.vcs;
                    rt -> m_isTemplate  = false;
                    rt->m_color         = rs.color.qString();
                    
                    if(!rcre)       // should only trigger for the root, avoid a nasty recursion
                        continue;
                    
                    QString     qf  = quill_fragment_for_dir(rpath);
                    if(!QFile::exists(qf)){
                        yDebug() << "Quill fragment " << qf << " does not exist (not required to).";
                        continue;
                    }
                    
                    QuillFile        sub;
                    if(!sub.load(qf.toStdString())){
                        yWarning() << "Unable to read Quill fragment: " << qf;
                        continue;
                    }
                    
                    
                    String  abbr            = sub.abbreviation();
                    if((!abbr.empty()) && (depth < spots.abbr)){
                        m.abbreviation      = abbr.qString();
                        spots.abbr          = depth;
                    }
                    String  author          = sub.author();
                    if((!author.empty()) && (depth < spots.author)){
                        m.author            = author.qString();
                        spots.author        = depth;
                    }
                    Copyright   copyright   = sub.copyright();
                    
                    if(copyright.from && (depth < spots.copyfrom)){
                        m.copyright.from    = copyright.from;
                        spots.copyfrom      = depth;
                    }
                    if((copyright.stance != AssertDeny()) && (depth < spots.copystance)){
                        m.copyright.stance  = copyright.stance;
                        spots.copystance    = depth;
                    }
                    if((!copyright.text.empty()) && (depth < spots.copytext)){
                        m.copyright.text    = copyright.text;
                        spots.copytext      = depth;
                    }
                    if(copyright.to && (depth < spots.copyto)){
                        m.copyright.to      = copyright.to;
                        spots.copyto        = depth;
                    }
                    String home = sub.home();
                    if((!home.empty()) && (depth < spots.home)){
                        m.home              = home.qString();
                        spots.home          = depth;
                    }
                    String  name = sub.name();
                    if((!name.empty()) && (depth < spots.name)){
                        m.name              = name.qString();
                        spots.name          = depth;
                    }
                    
                    _load(sub, rSeen, tSeen, pm2, tPolicy, spots, false, depth+1);
                }
            }
            return true;
        }

        /*! Gets/Creates a root
        */
        static std::pair<Root*,bool>   _root(const QString& s)
        {
            M&   m   = impl();
            const Root*r    = m.root_by_path.get(s, nullptr);
            if(r)
                return std::pair<Root*,bool>(const_cast<Root*>(r), false);
                
            Root*r2 = new Root(s);
            r2 -> m_id  = m.roots.size();
            m.roots << r2;
            m.root_by_path[s]    = r2;
            return std::pair<Root*,bool>(r2, true);
        }
    };


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    const QString&      abbreviation()
    {
        return impl().abbreviation;
    }
    
    const QString&      author()
    {
        return impl().author;
    }
    
    const Set<uint16_t>&    aux_ports()
    {
        return impl().aux_ports;
    }
    
    QString             best_guess(const QString&dirOrFragment)
    {
        QFileInfo fi(dirOrFragment);
        if(fi.fileName().toLower() == szQuillFile)
            return dirOrFragment;
        QDir    d(fi.isDir() ? dirOrFragment : fi.absolutePath());
        for(; !d.isRoot(); d.cdUp()){
            if(d.exists(szQuillFile))
                return d.absoluteFilePath(szQuillFile);
        }
        return QString();
    }

    QSqlDatabase        cache()
    {
        static thread_local QSqlDatabase s_ret  = connectDB();
        return s_ret;
    }
    
    const QString&      cache_db()
    {
        return impl().cache;
    }

    //Folder              config_folder()
    //{
        //return cdb::db_folder(cdb::top(), szConfigDir);
    //}   

#if 0
    QString             config_key(Document doc)
    {
        return config_key(cdb::key(doc));
    }

    QString             config_key(const QString& k)
    {
        QString r   = k;
        int d = r.indexOf('/');
        if((r > 0) && (k.at(0) == '.')){
            //  Strip out the ".config/";
            if(r.mid(0,d).toLower() == szConfigDir)
                r   = r.mid(d+1);
        }
        return strip_extension(r);
    }
#endif
    
    const Copyright&    copyright()
    {
        return impl().copyright;
    }

    const QString&     db_pid()
    {
        return impl().db_pid;
    }
    
    const QString&      dot()
    {
        return impl().dot;
    }

    
    const QString&      git()
    {
        return impl().git;
    }

    bool                has_init()
    {
        return impl().init;
    }

    const QString&      home()
    {
        return impl().home;
    }
    
    const QString&      hostname()
    {
        return impl().hostname;
    }
      
    const QString&      ini()
    {
        return impl().ini;
    }

    bool                initialize(const QString& dirOrFragment, Options opts)
    {
        return Init::_init(dirOrFragment, opts);
    }

    const QString&      local_user()
    {
        return impl().local_user;
    }
    
    const QString&      log_dir_path()
    {
        return impl().logs;
    }
    
    const QString&      markdown()
    {
        return impl().markdown;
    }
    
    const QString&      name()
    {
        return impl().name;
    }
    
    const QString&      perl()
    {
        return impl().perl;
    }

    unsigned short      port()
    {
        return impl().port;
    }
    
    const QDir&         quill_dir()
    {
        return impl().quill_dir;
    }

    QString             quill_fragment_for_dir(const QString&dirPath)
    {
        return QDir(dirPath).absoluteFilePath(szQuillFile);
    }

    const QString&      quill_key()
    {
        return impl().quill_key;
    }
    
    const QString&      quill_path()
    {
        return impl().quill_dir_path;
    }
    
    QString             quill_resolve(const char*z)
    {
        return impl().quill_dir.absoluteFilePath(z);
    }
    
    QString             quill_resolve(const QByteArray&z)
    {
        return quill_resolve(utf8(z));
    }
    
    QString             quill_resolve(const QString&z)
    {
        return impl().quill_dir.absoluteFilePath(z);
    }
    
    QString             quill_resolve(const std::string&z)
    {
        return quill_resolve(z.c_str());
    }
    
    unsigned int        read_timeout()
    {
        return impl().read_timeout;
    }
    
    const Root*         root(uint64_t rootId)
    {
        return impl().roots.value(rootId, nullptr);
    }
    
    const Root*         root(const QString& rootPath)
    {
        return impl().root_by_path.get(rootPath, nullptr);
    }
    
    uint64_t            root_count()
    {
        return impl().roots.size();
    }
    
    const Root*         root_first(DataRole dr)
    {
        return impl().root_first[dr];
    }

    const RoleMap&      root_firsts()
    {
        return impl().root_first;
    }
    
    const RootMap&      root_map()
    {
        return impl().root_by_path;
    }
    
    const RoleVecMap&   root_reads()
    {
        return impl().root_read;
    }

    const RootVector&   root_reads(DataRole dr)
    {
        return impl().root_read[dr];
    }
    
    const RoleVecMap&   root_writes()
    {
        return impl().root_write;
    }

    const RootVector&       root_writes(DataRole dr)
    {
        return impl().root_write[dr];
    }
    
    const RootVector&   roots()
    {
        return impl().roots;
    }
    
    const QString&      smartypants()
    {
        return impl().smartypants;
    }
    
    const QString&      start()
    {
        return impl().start;
    }
    
    QDateTime           start_time()
    {
        return impl().start_time;
    }
    
    const QString&      subversion()
    {
        return impl().subversion;
    }

    
    Vector<QString>     template_dir_paths(const QString& s)
    {
        Vector<QString>     ret;
        for(const QDir& d : impl().template_dirs){
            for(QString i : d.entryList(QStringList() << s, QDir::Dirs | QDir::NoDotAndDotDot))
                ret << d.absoluteFilePath(i);
        }
        return ret;
    }
    
    const QDirVector&   template_dirs()
    {
        return impl().template_dirs;
    }
    
    QDirVector          template_dirs(const QString& s)
    {
        Vector<QDir>        ret;
        for(const QString& d : template_dir_paths(s))
            ret << QDir(d);
        return ret;
    }
    
    //! Finds the quill associated with the template name
    QString             template_quill(const QString& s)
    {
        for(QDir d : template_dirs(s)){
            if(d.exists(szQuillFile))
                return d.absoluteFilePath(szQuillFile);
        }
        return QString();
    }
    
    const QStringSet&   templates()
    {
        return impl().templates;
    }
    
    const QStringSet&   templates_available()
    {
        return impl().templates_avail;
    }

    const QDir&         temp_dir()
    {
        return impl().temporary_dir;
    }
    
    const QString&      temp_dir_path()
    {
        return impl().temporary;
    }
    
    QString             temp_resolve(const char*z)
    {
        return impl().temporary_dir.absoluteFilePath(z);
    }
    
    QString             temp_resolve(const QByteArray&z)
    {
        return impl().temporary_dir.absoluteFilePath(utf8(z));
    }
    
    QString             temp_resolve(const QString&z)
    {
        return impl().temporary_dir.absoluteFilePath(z);
    }
    
    QString             temp_resolve(const std::string&z)
    {
        return impl().temporary_dir.absoluteFilePath(z.c_str());
    }
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

const Root*  Root::by_key(const QString&k)
{
    for(const Root* r : wksp::roots())  
        if(is_similar(r->key(),k))
            return r;
    return nullptr;
}

//  whether or not the provided string is a directory, or not
bool    Root::is_good_dir(const QString& s, Flag<G> f)
{
    QFileInfo       info(s);
    if(!info.exists()){
        if(f.is_set(G::Complain))
            yError() << "Non-existing directory '" << s << "' unable to load.";
        return false;
    }
    
    if(!info.isDir()){
        if(f.is_set(G::Complain))
            yError() << "Not a directory!  '" << s << "'";
        return false;
    }
    
    if(!info.isReadable()){
        if(f.is_set(G::Complain))
            yError() << "Unreadable directory '" << s << "'";
        return false;
    }

    if(f.is_set(G::Writable) && !info.isWritable()){
        if(f.is_set(G::Complain))
            yError() << "Unwritable directory '" << s << "'";
        return false;
    }
    
    if(!info.isExecutable()){
        if(f.is_set(G::Complain))
            yError() << "Non-executable directory '" << s << "'";
        return false;
    }
    
    if(info.isHidden()){
        if(f.is_set(G::Complain))
            yError() << "Hidden directory '" << s << "'";
        return false;
    }
    
    if(info.isRoot()){
        if(f.is_set(G::Complain))
            yError() << "Root directory! '" << s << "'";
        return false;
    }
    
    
    return true;
}



Root::Root(const QString&s) : m_path(s), m_dir(s), m_isTemplate(false), m_id(0)
{
    if(m_dir.exists(".svn"))
        m_vcs |= Vcs::SVN;
    if(m_dir.exists(".git"))
        m_vcs |= Vcs::GIT;
}

Root::Root(const QString&s, PolicyMap pm) : Root(s)
{
    for(DataRole dr : DataRole::all_values())
        m_roles[dr].access  = pm[dr];
}


Root::~Root()
{
}


//const Root*    Root::dirFor(const QStringList&sl) const
//{
    //const Root*    d   = this;
    //{
        //tbb::spin_rw_mutex::scoped_lock s_lock(m_mutex, false);
        //for(int i=0;d && (i<sl.size());++i){
            //d   = d -> m_sub.get( sl[i], nullptr);
        //}
    //}
    //return d;
//}

//Root*          Root::dirFor(const QStringList&sl, bool fCreate)
//{
    //Root*      d   = this;
    //{
        //tbb::spin_rw_mutex::scoped_lock s_lock(m_mutex, fCreate);
        //for(int i=0;d && (i<sl.size());++i){
            //QString     b   = sl[i];
            //Root*  e   = d -> m_sub.get( b, nullptr);
            //if(!e && fCreate){
                //e   = new Root(d, b);
                //d -> m_sub[b]   = e;
            //}
            //d   = e;
        //}
    //}
    //return d;
//}

//Vector<Directory>   Root::all_directories() const
//{
    //Vector<Directory>    ret;
    //static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Directories WHERE root=?");
    //auto s_af   = s.af();
    //s.bind(0, m_id);
    //if(s.exec()){
        //while(s.next())
            //ret << Directory(s.valueU64(0));
    //}
    //return ret;
//}

//uint64_t            Root::all_directory_count() const
//{
    //static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM Directories WHERE root=?");
    //auto s_af   = s.af();
    //s.bind(0, m_id);
    //if(s.exec() && s.next())
        //return s.valueU64(0);
    //return 0ULL;
//}

//Vector<Fragment>        Root::all_fragments() const
//{   
    //Vector<Fragment>    ret;
    //static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Fragments WHERE root=?");
    //auto s_af   = s.af();
    //s.bind(0, m_id);
    //if(s.exec()){
        //while(s.next())
            //ret << Fragment(s.valueU64(0));
    //}
    //return ret;
//}

//uint64_t    Root::all_fragment_count() const
//{
    //static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM Fragments WHERE root=?");
    //auto s_af   = s.af();
    //s.bind(0, m_id);
    //if(s.exec() && s.next())
        //return s.valueU64(0);
    //return 0ULL;
//}



bool        Root::exists(const char*z) const
{
    if(!z)
        return false;
    return exists(QString(z));
}

bool        Root::exists(const std::string&z) const
{
    return exists(QString::fromStdString(z));
}

bool        Root::exists(const QByteArray&z) const
{
    return exists(QString::fromUtf8(z));
}

bool        Root::exists(const QString&z) const
{
    return m_dir.exists(sanitize_path(z));
}

//Fragment        Root::fragment(const char*z) const
//{
    //return fragment(QString(z));
//}

//Fragment        Root::fragment(const QByteArray&z) const
//{
    //return fragment(QString::fromUtf8(z));
//}

//Fragment        Root::fragment(const QString&fname) const
//{
    //return Fragment::get(m_dir.absoluteFilePath(fname));
    ////QStringList path   = Fragment::sanitize(fname.split('/'));
    ////if(path.isEmpty() || path.last().isEmpty())
        ////return Fragment();
    ////return Fragment(this, m_dir.absoluteFilePath(path.join('/')), 0);
//}

//Fragment        Root::fragment(const std::string&z) const
//{
    //return fragment(QString::fromStdString(z));
//}

bool        Root::has_git() const
{
    return m_vcs.is_set(Vcs::GIT);
}

bool        Root::has_subversion() const
{
    return m_vcs.is_set(Vcs::SVN);
}

bool        Root::is_readable(DataRole dr) const
{
    switch(policy(dr)){
    case Access::Default:
    case Access::ReadOnly:
    case Access::ReadWrite:
    case Access::WriteFirst:
        return true;
    default:
        return false;
    }
}

bool        Root::is_writable(DataRole dr) const
{
    switch(policy(dr)){
    case Access::Default:
    case Access::ReadWrite:
    case Access::WriteFirst:
        return true;
    default:
        return false;
    }
}

bool        Root::is_write_first(DataRole dr) const
{
    return policy(dr) == Access::WriteFirst;
}


bool        Root::make_path(const char*z) const
{
    if(!z)
        return false;
    return make_path(QString(z));
}

bool        Root::make_path(const std::string&z) const
{
    return make_path(QString::fromStdString(z));
}

bool        Root::make_path(const QByteArray&z) const
{
    return make_path(utf8(z));
}

bool        Root::make_path(const QString&z) const
{
    return m_dir.mkpath(z);
}


Access      Root::policy(DataRole dr) const
{
    Access  a = m_roles[dr].access;
    //if((a == Access::Default) && (dr != DataRole::Global))
        //a   = m_roles[DataRole::Global].access;
    return a;
}

unsigned short       Root::read_order(DataRole dr) const
{
    return m_roles[dr].readOrder;
}


QString     Root::resolve(const char*z) const
{
    if(!z)
        return QString();
    return resolve(QString(z));
}

QString     Root::resolve(const QByteArray&z) const
{
    return resolve(QString::fromUtf8(z));
}

QString     Root::resolve(const QString&   z) const
{
    return m_dir.absoluteFilePath(sanitize_path(z));
}

QString     Root::resolve(const std::string&z) const
{
    return resolve(QString::fromStdString(z));
}

unsigned short  Root::write_order(DataRole dr) const
{
    return m_roles[dr].writeOrder;
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {

    String          g_name()
    {
        return wksp::name();
    }
    
    String          g_abbr()
    {
        return wksp::abbreviation();
    }
    
    INVOKE(
        global("abbreviation", g_abbr);
        global("name", g_name);
    );

}
