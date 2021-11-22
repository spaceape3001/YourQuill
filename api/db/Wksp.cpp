////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "CommonDirs.hpp"
#include "Image.hpp"
#include "Root.hpp"
#include "QuillFile.hpp"
#include "Wksp.hpp"

#include "WkspLogging.hpp"
#include "ShareDir.hpp"
#include <db/bit/Copyright.hpp>
#include <meta/Global.hpp>
#include <meta/GlobalImpl.hpp>
#include <util/CmdArgs.hpp>
#include <util/DelayInit.hpp>
#include <util/FileUtils.hpp>
#include <util/Strings.hpp>
#include <util/ThreadId.hpp>
#include <util/Utilities.hpp>

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTextCodec>

#include <pwd.h>
#include <filesystem>
#include <unistd.h>

    // hack because Qt #define emit for their MOC and tbb now uses it.
#ifdef emit
    #undef emit
#endif

#include <tbb/spin_rw_mutex.h>

using namespace wksp;

namespace {
    using namespace std::filesystem;
    
    static constexpr const char*    kShareDir   = YQ_SHARE_DIR;

    static constexpr const unsigned short       kDefaultPort        = 25505;
    static constexpr const unsigned int         kDefaultReadTimeout = 30000;
    static constexpr const mode_t   kDirMode        = 0755;
    

    using path_set      = Set<path,IgCase>;
    
    const char* homeDir()
    {
        const char* s   = getenv("HOME");
        if(s)
            return s;
        struct passwd*  pw = getpwuid(getuid());;
        if(pw)
            return pw -> pw_dir;
        return nullptr;
    }
    
    
    path   absolute_proximate(const String& spec, const path& base)
    {
        std::error_code ec1, ec2;
        return absolute(proximate(spec.c_str(), base, ec1), ec2);
    }
    
    
    struct SetSpots {
        unsigned int    abbr        = ~0;
        unsigned int    author      = ~0;
        unsigned int    copyfrom    = ~0;
        unsigned int    copystance  = ~0;
        unsigned int    copytext    = ~0;
        unsigned int    copyto      = ~0;
        unsigned int    home        = ~0;
        unsigned int    name        = ~0;
    };

    enum DirType {
        BadDir,
        ReadDir,
        WriteDir
    };
    
    DirType  dir_type(const std::filesystem::path& fs)
    {
        if(fs.filename().c_str()[0] == '.')  // filter out hidden
            return BadDir;
    
        std::error_code ec;
        if(!std::filesystem::is_directory(fs, ec))
            return BadDir;
        if(access(fs.c_str(), R_OK | W_OK | X_OK) == 0)
            return WriteDir;
        if(access(fs.c_str(), R_OK | X_OK) == 0)
            return ReadDir;
        return BadDir;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    

    struct M {
        String                  abbr;
        App                     app;
        String                  app_name;
        String                  author;
        Set<uint16_t>           aux;
        
        path                    broker_ipc;
        path                    build           = YQ_BUILD_ROOT;
        path                    db_pid;
        path                    cache;
        Copyright               copyright;
        path                    dot             = "/usr/bin/dot";
        path                    git             = "/usr/bin/git";
        String                  home;
        String                  host;
        path                    ini;
        bool                    init                = false;
        String                  luser;
        String                  logfrag;
        path                    logs;
        path                    markdown;
        String                  name;
        unsigned int            options;
        path                    perl            = "/usr/bin/perl";
        unsigned short          port            = 0;
        
        path                    qarg;
        path                    qdir;
        path                    qfile;
        String                  qkey;
        
        unsigned int            read_timeout        = 0;
        RoleMap                 rfirst;
        //Vector<String>          ricons;
        RootMap                 rkey;
        RootVector              roots;
        RootPathMap             rpath;
        RoleVecMap              rread;
        RoleVecMap              rwrite;
        path                    smartypants;
        path                    server_ipc;
        String                  start;
        std::time_t             start_time;
        path                    subversion      = "/usr/bin/svn";
        
        PathVector              shared_dirs;
        
        PathVector              template_dirs;
        StringSet               templates;
        StringSet               templates_avail;
        
        path                    tmp;
        path                    updater_ipc;
        
        
        mutable tbb::spin_rw_mutex  mutex;

        M()
        {
            std::time(&start_time);
            {
                std::tm     gt;
                gmtime_r(&start_time, &gt);
                char    buffer[256];
                strftime(buffer, sizeof(buffer), "yyyyMMdd-HHmmss", &gt);
                start   = buffer;
            }

            const char*   tmpdir  = getenv("TMPDIR");
            if(!tmpdir)
                tmpdir          = "/tmp";
            tmp                 = path(tmpdir) / "yquill";
            
            char        hname[HOST_NAME_MAX+1];
            gethostname(hname, sizeof(hname));
            hname[HOST_NAME_MAX]    = '\0';
            host            = String(hname);

            make_path(tmp / ".broker");
            broker_ipc          = tmp / ".broker" / "ipc";
        
            path   lHomeDir;
            const char* hdir   = homeDir();
            if(hdir){
                auto hd     = path(hdir) / ".yquill";
                if(!::access(hdir, W_OK))
                    make_path(lHomeDir);
                if(!::access(lHomeDir.c_str(), R_OK|X_OK))
                    shared_dirs << hd;
            }

                
            for(const char* z : { "/usr/local/share/yquill", "/usr/share/yquill", kShareDir }){
                if(!access(z, R_OK|X_OK))
                    shared_dirs << z;
            }

            template_dirs   = yqdir::all_children(shared_dirs, "template");
            templates_avail = yqdir::subdirectory_names_set(template_dirs);
            
            markdown        = yqdir::first_child(shared_dirs, "perl/Markdown.pl");
            smartypants     = yqdir::first_child(shared_dirs, "perl/SmartyPants.pl");
                
                //  While we have Qt, this will remain.
            QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
        }
    };
    
    M&  impl()
    {
        static M s_ret;
        return s_ret;
    }

    Vector<Root*>       _roots()
    {
        Vector<Root*>  ret;
        for(const Root* r : impl().roots)
            ret << const_cast<Root*>(r);
        return ret;
    }
    

    //  This will remain until we divorce Qt
    QSqlDatabase        connectDB()
    {
        QSqlDatabase    db      = QSqlDatabase::addDatabase("QSQLITE", QString("CacheDB %1").arg(thread_id()));
        db.setDatabaseName(impl().cache.c_str());
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




    //struct SetSpots {
        //unsigned int    abbr;
        //unsigned int    author;
        //unsigned int    copyfrom;
        //unsigned int    copystance;
        //unsigned int    copytext;
        //unsigned int    copyto;
        //unsigned int    home;
        //unsigned int    name;
        
        //SetSpots() : abbr(~0), author(~0), copyfrom(~0), 
                    //copystance(~0), copytext(~0), copyto(~0), 
                    //home(~0), name(~0) {}
    //};

    //QString     dirPathFor(const path& pth)
    //{
        //return QFileInfo( QString::fromStdString(pth.string())).absolutePath();
    //}
    
    path       best_guess_quill(const path& sv)
    {
        if(is_similar(sv.filename().c_str(), wksp::szQuillFile))
            return sv;
        path    rt  = sv.root_path();
        for(path q = is_directory(sv) ? sv : q.parent_path(); q != rt; q = q.parent_path()){
            path    f   = q / wksp::szQuillFile;
            if(exists(f))
                return f;
        }
        return path();  // failed to find.... abort
    }

}


namespace wksp {
    struct Init {   // Only a class to support "friend"
        static void rootList(const QString& pfx=QString())
        {
            const M&  m = impl();
            yInfo () << pfx << m.roots.size() << " roots";
            for(const Root* rt : m.roots)
                yInfo () << pfx << " " << rt->path;
        }
    
    
        static bool _init(const std::filesystem::path& arg, unsigned int opts, App app)
        {
            if(thread_id() != 0){
                wkspError << "Workspace *MUST* be initialized from the main thread!";
                return false;
            }

            M&  m       = impl();

            m.app       = app;
            m.app_name  = CmdArgs::appName();
            m.options   = opts;
            m.qarg      = arg;
            
            path    q       = arg;
            if(q.is_relative()){
                char        cwd[PATH_MAX+1];
                if(getcwd(cwd, PATH_MAX) != cwd)
                    return false;
                cwd[PATH_MAX]   = '\0';
                q   = path(cwd) / arg.c_str();
            }
            q = q.lexically_normal();

            if(opts & SEARCH){
                m.qfile = best_guess(q);
            } else {
                if(is_similar(q.filename().c_str(), szQuillFile)){
                    m.qfile   = q;
                } else if(is_directory(q)){
                    m.qfile   = q / szQuillFile;
                }
            }

            if(m.qfile.empty()){
                wkspError << "No " << szQuillFile << " fragment at specified location, REQUIRED!";
                return false;
            }
            
            m.qdir      = m.qfile.parent_path();
            m.qkey      = m.qdir.filename().c_str();

            if(dir_type(m.qdir) == BadDir){
                wkspError << "Bad directory for workspace --> " << m.qdir;
                return false;
            }
            
            QuillFile       doc;
            if(!doc.load(m.qfile)){
                wkspError << "Unable to load the file: " << m.qfile;
                return false;
            }
            
            if(doc.temp_dir.empty()){
                m.tmp   = m.tmp / m.qkey.c_str();
            } else 
                m.tmp   = absolute_proximate(doc.temp_dir, m.qdir);
            
            m.aux       = std::move(doc.aux_ports);
            m.port      = doc.port;
            if(!m.port)
                m.port      = kDefaultPort;
            m.read_timeout  = doc.read_timeout;
            if(!m.read_timeout)
                m.read_timeout  = kDefaultReadTimeout;
                
                
            if(!doc.log_dir.empty())
                m.logs        = absolute_proximate(doc.log_dir, m.tmp);
            else
                m.logs        = m.tmp / "log";
            make_path(m.logs);


            if(opts & INIT_LOG){
                String  lf  = String(m.app_name) + "-" + m.start + ".log";
                path log         = m.logs / lf.c_str();
                log_to_file(log.c_str(), LogPriority::All);
            }
            
            if(!doc.cache.empty())
                m.cache       = absolute_proximate(doc.cache, m.tmp);
            else
                m.cache       = m.tmp / "cache";
                

            StringSet        rSeen;
            Root*           rt  = new Root(m.qdir, PolicyMap(Access::ReadWrite));
            m.roots << rt;
            m.rpath["."]              = rt;
            m.rpath[m.qdir.c_str()]   = rt;
            m.rkey["."]             = rt;

            SetSpots        qs;
            m.abbr                  = doc.abbr;
            if(!m.abbr.empty())
                qs.abbr             = 0;
            m.author                = doc.author;
            if(!m.author.empty())
                qs.author           = 0;
            m.copyright             = doc.copyright;
            if(!m.copyright.from)
                qs.copyfrom         = 0;
            if(!m.copyright.stance)
                qs.copystance       = 0;
            if(!m.copyright.text.empty())
                qs.copytext         = 0;
            if(!m.copyright.to)
                qs.copyto           = 0;
            m.home                  = doc.home;
            if(!m.home.empty())
                qs.home            = 0;
            m.name                  = doc.name;
            if(!m.name.empty())
                qs.name          = 0;
            
             _load(doc, rSeen, PolicyMap(Access::ReadWrite), PolicyMap((opts&TEMPLATES_RO) ? Access::ReadOnly : Access::ReadWrite), false, 0, qs);
            

            m.roots.sort([](const Root* a, const Root* b){
                if(a->is_template != b->is_template)
                    return a->is_template < b->is_template;
                if(a->depth != b->depth)
                    return a->depth < b->depth;
                return a->id < b->id;
            });
            
            Vector<Root*>   eroots  = _roots();
            
            for(Root* r2 : eroots){
                fspath  cd      = r2 -> path / szConfigDir;
                mkdir(cd.c_str(), kDirMode);
            }
            
            StringSet   keys;
            uint64_t    i   = 0;
            for(Root* r2 : eroots){  // give everything IDs and record the keys
                r2 -> id = i++;
                if(dir_type(r2 -> path) != WriteDir){
                    for(DataRole dr : DataRole::all_values())
                        r2 -> access[dr]    = moderate(r2 -> access[dr], Access::ReadOnly);
                }
                keys << r2 -> key;
            }
            
            //  make sure everything has a key, default is the number
            for(Root* r2 : eroots){
                if(!r2->key.empty())
                    continue;
                String s   = String::number(r2->id);
                if(!keys.has(s)){
                    r2 -> key = s;
                    keys << s;
                }
                s += '_';
                for(i=0;; ++i){
                    String  k   = s + String::number(i);
                    if(!keys.has(k)){
                        r2 -> key = k;
                        keys << k;
                        break;
                    }
                }
            }
            
            //  Sort out the writers
            for(DataRole dr : DataRole::all_values()){
                bool    hasFirst    = false;
                for(Root* r3 : eroots){
                    if(r3->access[dr] == Access::WriteFirst){
                        if(hasFirst)    
                            r3->access[dr]   = Access::ReadWrite;
                        else
                            hasFirst    = true;
                    }
                }

                if(!hasFirst){
                    for(Root* r3 : eroots){
                        if(r3->access[dr]     == Access::WriteFirst) {
                            r3 -> access[dr]    = Access::WriteFirst;
                            hasFirst    = true;
                            break;
                        }
                    }
                }

                if(!hasFirst)
                    m.rfirst[dr]   = nullptr;

                for(Root* r3 : eroots){
                    switch(r3->access[dr]){
                    case Access::WriteFirst:
                        m.rfirst[dr]     = r3;
                        /* fall through */
                    case Access::ReadWrite:
                        m.rwrite[dr] << r3;
                    case Access::ReadOnly:
                        m.rread[dr] << r3;
                    case Access::NoAccess:
                    default:
                        break;
                    }
                }
            }

            m.init        = true;
            return true;
        }

            
        static void      _load(const QuillFile&doc, StringSet& rSeen, PolicyMap rPolicy, PolicyMap tPolicy, bool fTemplate, unsigned depth, SetSpots&qs)
        {   
yError() << "Init::_load(" << doc.file() << ")";        
        
            M& m = impl();
            path    qdir    = doc.file().parent_path();
            for(auto& rs : doc.templates){
                if(!m.templates.add(rs.path))
                    continue;
                PolicyMap   pm2 = moderate(tPolicy, rs.policy);
                for(const path& t : template_dirs(rs.path)){
                    Root*   rt          = nullptr;
                    bool    rcre        = false;
                    std::tie(rt,rcre)   = _root(t);
                    if(rcre)
                        rt -> depth   = depth;
                    for(DataRole dr : DataRole::all_values()){
                        if(pm2[dr] == Access::Default)
                            pm2[dr] = tPolicy[dr];
                        rt -> access[dr]     = pm2[dr];
                    }
                    rt->name            =  ":" + rs.path;
                    rt->key             = rs.key;
                    if(rt->key.empty())
                        rt->key         = rt->name;
                    rt->is_template     = true;
                    rt->color           = rs.color;
                    rt->icon            = rs.icon;
                    
                    path    qf  = quill_fragment_for_dir(t);
                    if(!std::filesystem::exists(qf)){
                        wkspDebug << "Quill fragment " << qf << " does not exist (not required to).";
                        continue;
                    }
                    
                    QuillFile        sub;
                    if(!sub.load(qf)){
                        wkspWarning << "Unable to read Quill fragment: " << qf;
                        continue;
                    }
                    
                    _load(sub, rSeen, rPolicy, pm2, true, depth+1, qs);
                }
            }
                
            if(!fTemplate){
                for(auto& rs : doc.roots){
                            // TODO ... need the equivalent to the QDIr magic....
                    path    rpath   = rs.path.c_str(); // absolute_proximate( rs.path, qdir);
                    if(rpath == ".")
                        rpath       = qdir;
                    if(!rSeen.add(rpath.string()))       // already seen
                        continue;

                    PolicyMap   pm2 = moderate(rPolicy, rs.policy);    
                    
                    Root*   rt      = nullptr;
                    bool    rcre    = false;
                    std::tie(rt,rcre)   = _root(rpath);
                    if(rcre)
                        rt -> depth   = depth;
                        
                    for(DataRole dr : DataRole::all_values()){
                        if(pm2[dr] == Access::Default)
                            pm2[dr] = rPolicy[dr];
                        rt -> access[dr]    = pm2[dr];
                    }
                    rt -> name        = rs.name;
                    rt -> key         = rs.key;
                    if(rt -> key.empty())
                        rt -> key     = rt -> name;
                    if(rs.vcs != Vcs())
                        rt -> vcs    |= rs.vcs;
                    rt -> is_template  = false;
                    rt->color         = rs.color.qString();
                    rt->icon          = rs.icon.qString();
                    
                    if(!rcre)       // should only trigger for the root, avoid a nasty recursion
                        continue;
                        
                    path     qf  = quill_fragment_for_dir(rpath);
                    if(!std::filesystem::exists(qf)){
                        wkspDebug << "Quill fragment " << qf << " does not exist (not required to).";
                        continue;
                    }
                    
                    QuillFile        sub;
                    if(!sub.load(qf)){
                        wkspWarning << "Unable to read Quill fragment: " << qf;
                        continue;
                    }
                    
                    
                    if((!sub.abbr.empty()) && (depth < qs.abbr)){
                        m.abbr             = sub.abbr;
                        qs.abbr             = depth;
                    }
                    if((!sub.author.empty()) && (depth < qs.author)){
                        m.author            = sub.author;
                        qs.author           = depth;
                    }
                    
                    if(sub.copyright.from && (depth < qs.copyfrom)){
                        m.copyright.from    = sub.copyright.from;
                        qs.copyfrom         = depth;
                    }
                    if((sub.copyright.stance != AssertDeny()) && (depth < qs.copystance)){
                        m.copyright.stance  = sub.copyright.stance;
                        qs.copystance       = depth;
                    }
                    if((!sub.copyright.text.empty()) && (depth < qs.copytext)){
                        m.copyright.text    = sub.copyright.text;
                        qs.copytext         = depth;
                    }
                    if(sub.copyright.to && (depth < qs.copyto)){
                        m.copyright.to      = sub.copyright.to;
                        qs.copyto        =   depth;
                    }
                    if((!sub.home.empty()) && (depth < qs.home)){
                        m.home              = sub.home;
                        qs.home             = depth;
                    }
                    if((!sub.name.empty()) && (depth < qs.name)){
                        m.name              = sub.name;
                        qs.name             = depth;
                    }
                    
                    _load(sub, rSeen, pm2, tPolicy, false, depth+1, qs);
                }
            }
        }



        /*! Gets/Creates a root
        */
        static std::pair<Root*,bool>   _root(const path& s)
        {
            M&   m   = impl();
            const Root*r    = m.rpath.get(s, nullptr);
            if(r)
                return std::pair<Root*,bool>(const_cast<Root*>(r), false);
                
            Root*r2 = new Root(s);
            r2 -> id  = m.roots.size();
            m.roots << r2;
            m.rpath[s]    = r2;
            return std::pair<Root*,bool>(r2, true);
        }
    };


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    const String&       abbreviation()
    {
        return impl().abbr;
    }
    
    const String&       author()
    {
        return impl().author;
    }
    
    const Set<uint16_t>&    aux_ports()
    {
        return impl().aux;
    }
    
    std::filesystem::path           best_guess(const std::filesystem::path& dirOrFragment)
    {
        return best_guess_quill(dirOrFragment);
    }

    QSqlDatabase        cache()
    {
        static thread_local QSqlDatabase s_ret  = connectDB();
        return s_ret;
    }
    
    const std::filesystem::path&    cache_db()
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

    const std::filesystem::path&    db_pid()
    {
        return impl().db_pid;
    }
    
    const std::filesystem::path&    dot()
    {
        return impl().dot;
    }

    const std::filesystem::path&    git()
    {
        return impl().git;
    }

    bool                has_init()
    {
        return impl().init;
    }

    const String&       home()
    {
        return impl().home;
    }
    
    const String&   host()
    {
        return impl().host;
    }
    
      
    const std::filesystem::path&    ini()
    {
        return impl().ini;
    }

    bool                initialize(const std::filesystem::path&dirOrFragment, unsigned int opts, App app)
    {
        static bool ret = Init::_init(dirOrFragment, opts, app);
        return ret;
    }

    const String&      local_user()
    {
        return impl().luser;
    }
    
    const std::filesystem::path&    log_dir()
    {
        return impl().logs;
    }
    
    const std::filesystem::path&    markdown()
    {
        return impl().markdown;
    }
    
    const String&       name()
    {
        return impl().name;
    }
    
    const std::filesystem::path&    perl()
    {
        return impl().perl;
    }

    unsigned short      port()
    {
        return impl().port;
    }
    
    const std::filesystem::path&    quill_dir()
    {
        return impl().qdir;
    }

    const std::filesystem::path&    quill_file()
    {
        return impl().qfile;
    }

    std::filesystem::path           quill_fragment_for_dir(const std::filesystem::path& pth)
    {
        return pth / szQuillFile;
    }

    const String&       quill_key()
    {
        return impl().qkey;
    }
    
    std::filesystem::path   quill_resolve(const char* z)
    {
        return quill_resolve(std::filesystem::path(z));
    }
    
    std::filesystem::path   quill_resolve(const QByteArray&z)
    {
        return quill_resolve(std::filesystem::path(z.constData()));
    }
    
    std::filesystem::path   quill_resolve(const QString&z)
    {
        return quill_resolve(std::filesystem::path(z.toStdString()));
    }
    
    std::filesystem::path   quill_resolve(const std::string&z)
    {
        return quill_resolve(std::filesystem::path(z));
    }
    
    std::filesystem::path   quill_resolve(const std::string_view&z)
    {
        return quill_resolve(std::filesystem::path(z));
    }
    
    std::filesystem::path   quill_resolve(const std::filesystem::path&z)
    {
        return impl().qdir / z;
    }

    
    unsigned int        read_timeout()
    {
        return impl().read_timeout;
    }
    
    const Root*         root(uint64_t rootId)
    {
        return impl().roots.value(rootId, nullptr);
    }
    
    const Root*    root(const std::string_view& key)
    {
        return impl().rkey.get(key, nullptr);
    }
    
    uint64_t            root_count()
    {
        return impl().roots.size();
    }
    
    const Root*         root_first(DataRole dr)
    {
        return impl().rfirst[dr];
    }

    const RoleMap&      root_firsts()
    {
        return impl().rfirst;
    }
    
    const RootPathMap&  root_path_map()
    {
        return impl().rpath;
    }
    
    const RoleVecMap&   root_reads()
    {
        return impl().rread;
    }

    const RootVector&   root_reads(DataRole dr)
    {
        return impl().rread[dr];
    }
    
    const RoleVecMap&   root_writes()
    {
        return impl().rwrite;
    }

    const RootVector&       root_writes(DataRole dr)
    {
        return impl().rwrite[dr];
    }
    
    const RootVector&   roots()
    {
        return impl().roots;
    }
    
    const std::filesystem::path&    smartypants()
    {
        return impl().smartypants;
    }
    
    const String&      start()
    {
        return impl().start;
    }
    
    std::time_t        start_time()
    {
        return impl().start_time;
    }
    
    const std::filesystem::path&    subversion()
    {
        return impl().subversion;
    }

    const PathVector&   template_dirs()
    {
        return impl().template_dirs;
    }
    

    PathVector          template_dirs(const std::string_view& templateName)
    {
        PathVector  ret;
        for(const path& p : impl().template_dirs){
            path    p2  = p / templateName;
            if(dir_type(p2) != BadDir)
                ret << p2;
        }
        return ret;
    }
    
    //! Finds the quill associated with the template name
    std::filesystem::path   template_quill(const std::string_view& templateName)
    {
        PathVector  ret;
        for(const path& p : impl().template_dirs){
            path    p2  = p / templateName;
            if(dir_type(p2) == BadDir)
                continue;
            p2  /= szQuillFile;
            if(std::filesystem::exists(p2))
                return p2;
        }
        return {};
    }
    
    const StringSet&   templates()
    {
        return impl().templates;
    }
    
    const StringSet&   templates_available()
    {
        return impl().templates_avail;
    }

    const std::filesystem::path&         temp_dir()
    {
        return impl().tmp;
    }

    std::filesystem::path           temp_resolve(const char* z)
    {
        return temp_resolve(path(z));
    }
    
    std::filesystem::path           temp_resolve(const QByteArray&z)
    {
        return temp_resolve(path(z.constData()));
    }
    
    std::filesystem::path           temp_resolve(const QString&z)
    {
        return temp_resolve(path(z.toStdString()));
    }
    
    std::filesystem::path           temp_resolve(const std::string&z)
    {
        return temp_resolve(path(z));
    }
    
    std::filesystem::path           temp_resolve(const std::string_view&z)
    {
        return temp_resolve(path(z));
    }
    
    std::filesystem::path           temp_resolve(const std::filesystem::path& p)
    {
        return impl().tmp / p;
    }
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

const Root*  Root::by_key(const std::string_view&k)
{
    for(const Root* r : wksp::roots())  
        if(is_similar(r->key,k))
            return r;
    return nullptr;
}



Root::Root(const std::filesystem::path&s) : path(s)
{
    if(yqdir::child_exists(s, ".svn"))
        vcs |= Vcs::SVN;
    if(yqdir::child_exists(s, ".git"))
        vcs |= Vcs::GIT;
}

Root::Root(const std::filesystem::path&s, PolicyMap pm) : Root(s)
{
    access  = pm;
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

bool    Root::exists(const char*z) const
{
    if(!z)
        return false;
    return exists(std::filesystem::path(z));
}

bool    Root::exists(const QString&z) const
{
    return exists(std::filesystem::path(z.toStdString()));
}

bool    Root::exists(const QByteArray&z) const
{
    return exists(std::filesystem::path(z.constData()));
}

bool    Root::exists(const std::string&z) const
{
    return exists(std::filesystem::path(z));
}

bool    Root::exists(const std::string_view&z) const
{
    return exists(std::filesystem::path(z));
}

bool    Root::exists(const std::filesystem::path&z) const
{
    return std::filesystem::exists(resolve(z));
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
    return vcs.is_set(Vcs::GIT);
}

bool        Root::has_subversion() const
{
    return vcs.is_set(Vcs::SVN);
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
    return make_path(std::filesystem::path(z));
}

bool        Root::make_path(const QByteArray&z) const
{
    return make_path(std::filesystem::path(z.constData()));
}

bool        Root::make_path(const QString&z) const
{
    return make_path(std::filesystem::path(z.toStdString()));
}

bool        Root::make_path(const std::string&z) const
{
    return make_path(std::filesystem::path(z));
}

bool        Root::make_path(const std::string_view&z) const
{
    return make_path(std::filesystem::path(z));
}

bool        Root::make_path(const std::filesystem::path&z) const
{
    return make_parent_path(resolve(z));
}


Access      Root::policy(DataRole dr) const
{
    return access.get(dr);
}

//unsigned short       Root::read_order(DataRole dr) const
//{
    //return m_roles[dr].readOrder;
//}


std::filesystem::path   Root::resolve(const char*  z   ) const
{
    if(!z)
        return {};
    return resolve(std::filesystem::path(z));
}

std::filesystem::path   Root::resolve(const QByteArray& z) const
{
    return resolve(std::filesystem::path(z.constData()));
}

std::filesystem::path   Root::resolve(const QString& z) const
{
    return resolve(std::filesystem::path(z.toStdString()));
}

std::filesystem::path   Root::resolve(const std::string& z) const
{
    return resolve(std::filesystem::path(z));
}

std::filesystem::path   Root::resolve(const std::string_view& z) const
{
    return resolve(std::filesystem::path(z));
}

std::filesystem::path   Root::resolve(const std::filesystem::path& z) const
{
    return path / z;
}


//unsigned short  Root::write_order(DataRole dr) const
//{
    //return m_roles[dr].writeOrder;
//}


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
