////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Workspace.hpp"

#include <yq/core/BasicApp.hpp>
#include <yq/meta/Init.hpp>

#include <yq/core/DelayInit.hpp>
#include <yq/core/Logging.hpp>
#include <yq/core/ThreadId.hpp>
#include <yq/container/set_utils.hpp>
#include <yq/file/DirUtils.hpp>
#include <yq/file/FileUtils.hpp>
#include <yq/sql/SqlLite.hpp>
#include <yq/text/match.hpp>
#include <yq/text/vsplit.hpp>
#include <yq/typedef/string.hpp>

//#include <mithril/config/DirConfig.hpp>

#include <mithril/directories.hpp>
#include <mithril/root/Root.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/quill/QuillFile.hpp>

#include <atomic>
#include <pwd.h>
#include <unistd.h>
#include <limits.h>

#define wkspAlert           yAlert("wksp")
#define wkspCritical        yCritical("wksp")
#define wkspDebug           yDebug("wksp")
#define wkspError           yError("wksp")
#define wkspEmergency       yEmergency("wksp")
#define wkspFatal           yFatal("wksp")
#define wkspInfo            yInfo("wksp")
#define wkspNotice          yNotice("wksp")
#define wkspWarning         yWarning("wksp")

namespace yq::mithril::wksp {
    namespace {
        static constexpr const unsigned short       kDefaultPort        = 25505;
        static constexpr const unsigned int         kDefaultReadTimeout = 30000;
        static constexpr const mode_t               kDirMode            = 0755;



        std::filesystem::path   absolute_me(const std::filesystem::path& sv)
        {
            if(!sv.is_relative())
                return sv;
            
            char        cwd[PATH_MAX+1];
            if(getcwd(cwd, PATH_MAX) != cwd)
                return std::filesystem::path();
                
            cwd[PATH_MAX]   = '\0';
            return std::filesystem::path(cwd) / sv.c_str();
        }
        
        std::filesystem::path   search_up(const std::filesystem::path& sv)
        {
            if(is_similar(sv.filename().c_str(), szQuillFile))
                return sv;
            std::filesystem::path    rt  = sv.root_path();
            for(std::filesystem::path q = is_directory(sv) ? sv : sv.parent_path(); q != rt; q = q.parent_path()){
                std::filesystem::path    f   = q / szQuillFile;
                if(!::access(f.c_str(), F_OK|R_OK))
                    return f;
            }
            return std::filesystem::path();  // failed to find.... abort
        }
        
        std::filesystem::path   resolve(const std::filesystem::path& sv, bool searchFlag)
        {
            std::filesystem::path   q   = absolute_me(sv).lexically_normal();
            if(searchFlag)
                return search_up(q);
            if(is_similar(q.filename().c_str(), szQuillFile)){
                return q;
            } else if(std::filesystem::is_directory(q))
                return q / szQuillFile;
            return std::filesystem::path();
        }

        std::filesystem::path   absolute_proximate(const std::string& spec, const std::filesystem::path& base)
        {
            std::error_code ec1, ec2;
            return std::filesystem::absolute(std::filesystem::proximate(spec.c_str(), base, ec1), ec2);
        }

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

        std::filesystem::path       read_file_from([[maybe_unused]] uint16_t port)
        {
            return std::filesystem::path(); // TODO (it'll use CURL, path TBD)
        }

        std::filesystem::path       read_file_from(uint16_t port, const Set<uint16_t>& aux)
        {
            std::filesystem::path   q   = read_file_from(port);
            if(!q.empty())
                return q;
            
            for(uint16_t p : aux){
                q   = read_file_from(p);
                if(!q.empty())
                    return q;
            }
            
            return std::filesystem::path();
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
    }

    struct Impl {
        std::string             abbr;           // Abbrievation of the workspace
        App                     app = App{};    // Appliation type
        std::string             app_name;       // Name of application
        std::string             author;         // Author of the workspace
        Set<uint16_t>           aux;            // Auxillary ports
        StringSet               available;      // Available templates
        std::filesystem::path   cache;          // Cache location
        Copyright               copyright;      // Copyright info of the workspace
        FNDbFlags               db_flags = nullptr;
        std::atomic<bool>       db_init;
        std::filesystem::path   dot;            // DOT excutable
        std::filesystem::path   git;            // GIT executable
        std::string             home;           // Start page??? of the workspace
        std::string             host;           // Our hostname
        bool                    init = false;   // Our init flag
        std::filesystem::path   log;            // Our log
        std::filesystem::path   logs;           // log directory
        std::string             luser;          // local user
        std::filesystem::path   markdown;       // Markdown location
        std::string             name;           // Workspace name
        Oracle                  oracle;
        std::filesystem::path   perl;           // Perl path
        //std::filesystem::path   pid;            // PID file location?
        uint16_t                port = 0;       // Our designated port number
        std::filesystem::path   qdir;           // Master quill directory
        std::filesystem::path   qfile;          // Quill file (full)
        std::string             qkey;           // Quill key
        unsigned int            qoptions = 0;
        std::filesystem::path   qspec;          // What's provided
        root_vector_t           root_dirs;          // ALL root_dirs active in this wokrspace
        root_role_map_t         rfirst;         // First-write root_dirs (will be the first item in rwrite -- can be NULL)
        root_map_t              rkey;           // RootDir by key
        root_path_map_t         rpath;          // RootDir by path
        root_role_vec_map_t     rread;          // RootDir read order based on the data role
        unsigned int            rtimeout = 0;   // Read timeout on HTTP server
        root_role_vec_map_t     rwrite;         // RootDir write order based on te data role
        path_vector_t           shared_dirs;    // Shared directories
        std::filesystem::path   smartypants;    // Smarthpants script location
        std::time_t             start;          // Start time
        std::string             start_gui;      // GUI display
        std::string             start_file;     // FILE style
        std::filesystem::path   subversion;     // Subversion executable location
        path_vector_t           template_dirs;  // Directories of templates....
        StringSet               templates;      // Templates in use
        unsigned int            threads = 0;    // Desired thread count
        std::filesystem::path   tmp;            // Temporary directory
        
        
        Impl()
        {
            db_init = false;
        }
        
        
        bool    do_init(const Config& cfg);
        void    load(const QuillFile&doc, StringSet& rSeen, PolicyMap rPolicy, PolicyMap tPolicy, bool fTemplate, unsigned depth, SetSpots&qs);

        std::pair<RootDir*, bool>  make_root(const std::filesystem::path&);
        path_vector_t           template_directories(const std::string_view&);
        
        Vector<RootDir*>           editable_roots();
        
    };
    
    namespace {
        Impl&   impl()
        {
            static Impl s_impl;
            return s_impl;
        }
    }

    bool                has_init()
    {
        return impl().init;
    }

    std::filesystem::path   best_guess(const std::filesystem::path& sv)
    {
        return search_up(absolute_me(sv).lexically_normal());
    }

    bool    initialize(const Config&cfg)
    {
        static const bool ret = impl().do_init(cfg);
        return ret;
    }
    

    SqlLite&                        db()
    {
        static thread_local SqlLite     ret;
        if(!ret.is_open()){
            auto& i = impl();
            int f   = 0;
            if(i.db_flags){
                f   = i.db_flags();
            } else {
                f   = SqlLite::Create | SqlLite::ReadWrite;
            }
            if(!ret.open(i.cache, f)){
                wkspError << "Unable to open the database " << i.cache;
            }
        }
        return ret;
    }

    //  ================================================================
    //   INITIALIZATION/IMPLEMENTATION
    //  ================================================================

    
    bool                Impl::do_init(const Config& cfg)
    {
        if(thread::id() != 0){
            wkspError << "Workspace *MUST* be initialized from the main thread!";
            return false;
        }

        std::time(&start);
        struct tm   gt;
        gmtime_r(&start, &gt);
        
        char    timestamp[256];
        strftime(timestamp, sizeof(timestamp),  "%Y-%m-%d %H:%M:%S", &gt);
        start_gui       = timestamp;
        strftime(timestamp, sizeof(timestamp),  "%Y%m%d-%H%M%S", &gt);
        start_file      = timestamp;
                    
        host            = BasicApp::hostname();
        tmp             = BasicApp::temp_dir();


        for(const std::filesystem::path& p : cfg.share_dirs){
            if(!access(p.c_str(), R_OK|X_OK))
                shared_dirs.push_back(p);
        }

        //  build out share directories
        const std::filesystem::path& hdir   = BasicApp::user_home();
        if(!hdir.empty()){
            auto hd     = hdir / ".yquill";
            if(!::access(hdir.c_str(), W_OK)){
                make_path(hd);
            }
            if(!::access(hd.c_str(), R_OK|X_OK))
                shared_dirs.push_back(hd);
        }
        
        #ifdef DEV_BUILD
            const char* sd  = kernel::share_directories();
            vsplit(std::string_view(sd), ';', [&](std::string_view v){
                std::filesystem::path   p(v);
                if(!access(p.c_str(), X_OK|R_OK))
                    shared_dirs.push_back(p);
            });
        #endif

        #ifdef __unix__
        for(const char* z : { "/usr/local/share/yquill", "/usr/share/yquill"} ){
            if(!access(z, R_OK|X_OK))
                shared_dirs.push_back(z);
        }
        #endif

        template_dirs   = dir::all_children(shared_dirs, "template");

        auto xavail     = dir::subdirectory_names_set(template_dirs);
        available       = make_set(xavail, IgCase());
        auto perl_dirs  = dir::all_children(shared_dirs, "perl");
        
        markdown        = dir::first_child(perl_dirs, "Markdown.pl");
        smartypants     = dir::first_child(perl_dirs, "SmartyPants.pl");
        
        #ifdef __unix__
            dot         = BasicApp::find_exe("dot");
            git         = BasicApp::find_exe("git");
            perl        = BasicApp::find_exe("perl");
            subversion  = BasicApp::find_exe("svn");
        #else
            //  TODO WINDOWS -- obvious from above
        #endif
        
        app             = cfg.app;
        app_name        = BasicApp::app_name();
        qspec           = cfg.spec;
        qoptions        = cfg.options;
        db_flags        = cfg.db_flags;
        
        std::filesystem::path   q   = resolve(cfg.spec, static_cast<bool>(qoptions & SEARCH));
        
        QuillFile       doc;
        if(doc.load(q) != std::error_code()){
            wkspError << "Unable to load the file: " << q;
            return false;
        }
        
        if(qoptions & AUX_INIT){
            if(!doc.port)
                doc.port        = kDefaultPort;
        
            std::filesystem::path   p   = read_file_from(doc.port, doc.aux_ports);
            if(!p.empty()){
                doc.clear();
                if(!doc.load(p)) [[unlikely]] {
                    wkspError << "Unable to load the file: " << p;
                    return false;
                }
                q       = p;
            }
        }
        
        qfile           = q;
        qdir            = qfile.parent_path();
        qkey            = qdir.filename().c_str();
        
        if(dir_type(qdir) == BadDir) [[unlikely]] {
            wkspError << "Bad directory for workspace --> " << qdir;
            return false;
        }
            
        aux             = std::move(doc.aux_ports);
        port            = doc.port;
        if(!port)
            port        = kDefaultPort;
        rtimeout        = doc.read_timeout;
        if(!rtimeout)
            rtimeout    = kDefaultReadTimeout;
        threads         = doc.threads;

        if(doc.temp_dir.empty()){
            tmp         = tmp / qkey.c_str();
        } else
            tmp         = absolute_proximate(doc.temp_dir, qdir);

        if(doc.log_dir.empty()){
            logs        = tmp / "log";
        } else
            logs        = absolute_proximate(doc.log_dir, tmp);
        make_path(logs);
        
        if(qoptions & INIT_LOG){
            std::string lf = app_name + '-' + start_file + ".log";
            log = logs / lf.c_str();
            log_to_file(log.c_str(), LogPriority::All); // ALL for now
        }

        if(doc.cache.empty()){
            cache       = tmp / "cache";
        } else
            cache       = absolute_proximate(doc.cache, tmp);

        oracle.type = Oracle::DB;   // TODO ... curl

        StringSet    rSeen;
        RootDir*           rt  = new RootDir(qdir, PolicyMap(Access::ReadWrite));
        root_dirs << rt;
        rpath["."sv]              = rt;
        rpath[qdir]             = rt;
        rkey["."sv]               = rt;

        SetSpots        qs;
        abbr                    = doc.abbr;
        if(!abbr.empty())
            qs.abbr             = 0;
        author                  = doc.author;
        if(!author.empty())
            qs.author           = 0;
        copyright               = doc.copyright;
        if(!copyright.from)
            qs.copyfrom         = 0;
        if(!copyright.stance)
            qs.copystance       = 0;
        if(!copyright.text.empty())
            qs.copytext         = 0;
        if(!copyright.to)
            qs.copyto           = 0;
        home                  = doc.home;
        if(!home.empty())
            qs.home             = 0;
        name                  = doc.name;
        if(!name.empty())
            qs.name             = 0;
        
        load(doc, rSeen, PolicyMap(Access::ReadWrite), PolicyMap((qoptions&TEMPLATES_RO) ? Access::ReadOnly : Access::ReadWrite), false, 0, qs);
        

        root_dirs.sort([](const RootDir* a, const RootDir* b){
            if(a->is_template != b->is_template)
                return a->is_template < b->is_template;
            if(a->depth != b->depth)
                return a->depth < b->depth;
            return a->id < b->id;
        });
        
        Vector<RootDir*>   eroots  = editable_roots();
        
        for(RootDir* r2 : eroots){
            std::filesystem::path  cd      = r2 -> path / szConfigDir;
            mkdir(cd.c_str(), kDirMode);
        }
        
        StringSet   keys;
        uint64_t    i   = 0;
        for(RootDir* r2 : eroots){  // give everything IDs and record the keys
            r2 -> id = ++i;
            if(dir_type(r2 -> path) != WriteDir){
                for(DataRole dr : DataRole::all_values())
                    r2 -> access[dr]    = moderate(r2 -> access[dr], Access::ReadOnly);
            }
            keys << r2 -> key;
        }
        
        //  make sure everything has a key, default is the number
        for(RootDir* r2 : eroots){
            if(!r2->key.empty())
                continue;
            std::string s   = to_string(r2->id);
            if(!keys.has(s)){
                r2 -> key = s;
                keys << s;
                rkey[s] = r2;
                continue;
            }
            s += '_';
            for(i=0;; ++i){
                std::string  k   = s + to_string(i);
                if(!keys.has(k)){
                    r2 -> key = k;
                    keys << k;
                    rkey[k] = r2;
                    break;
                }
            }
        }
        
        //  Sort out the writers
        for(DataRole dr : DataRole::all_values()){
            bool    hasFirst    = false;
            for(RootDir* r3 : eroots){
                if(r3->access[dr] == Access::WriteFirst){
                    if(hasFirst)    
                        r3->access[dr]   = Access::ReadWrite;
                    else
                        hasFirst    = true;
                }
            }

            if(!hasFirst){
                for(RootDir* r3 : eroots){
                    if(r3->access[dr]     == Access::WriteFirst) {
                        r3 -> access[dr]    = Access::WriteFirst;
                        hasFirst    = true;
                        break;
                    }
                }
            }

            if(!hasFirst)
                rfirst[dr]   = nullptr;

            for(RootDir* r3 : eroots){
                switch(r3->access[dr]){
                case Access::WriteFirst:
                    rfirst[dr]     = r3;
                    /* fall through */
                case Access::ReadWrite:
                    rwrite[dr] << r3;
                case Access::ReadOnly:
                    rread[dr] << r3;
                case Access::NoAccess:
                default:
                    break;
                }
            }
        }

        init        = true;
        return true;
    }

    Vector<RootDir*>       Impl::editable_roots()
    {
        Vector<RootDir*>  ret;
        for(const RootDir* r : root_dirs)
            ret << const_cast<RootDir*>(r);
        return ret;
    }

    void    Impl::load(const QuillFile&doc, StringSet& rSeen, PolicyMap rPolicy, PolicyMap tPolicy, bool fTemplate, unsigned depth, SetSpots&qs)
    {
        std::filesystem::path    dir    = doc.file().parent_path();
        for(auto& rs : doc.templates){
            if(!templates.add(rs.path))
                continue;
                
            PolicyMap   pm2 = moderate(tPolicy, rs.policy);
            for(const std::filesystem::path& t : template_directories(rs.path)){
                RootDir*   rt          = nullptr;
                bool    rcre        = false;
                std::tie(rt,rcre)   = make_root(t);
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
                
                std::filesystem::path    qf  = t / szQuillFile;
                if(!std::filesystem::exists(qf)){
                    wkspDebug << "Quill fragment " << qf << " does not exist (not required to).";
                    continue;
                }
                
                QuillFile        sub;
                if(sub.load(qf) != std::error_code()){
                    wkspWarning << "Unable to read Quill fragment: " << qf;
                    continue;
                }
                
                load(sub, rSeen, rPolicy, pm2, true, depth+1, qs);
            }
        }
            
        if(!fTemplate){
            for(auto& rs : doc.root_dirs){
                        // TODO ... need the equivalent to the QDir magic....
                std::filesystem::path    rpp   = rs.path.c_str(); // absolute_proximate( rs.path, qdir);
                if(rpp == ".")
                    rpp       = dir;
                if(!rSeen.add(rpp.string()))       // already seen
                    continue;

                PolicyMap   pm2 = moderate(rPolicy, rs.policy);    
                
                RootDir*   rt      = nullptr;
                bool    rcre    = false;
                std::tie(rt,rcre)   = make_root(rpp);
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
                rt->color         = rs.color;
                rt->icon          = rs.icon;
                
                if(!rcre)       // should only trigger for the root_dir, avoid a nasty recursion
                    continue;
                    
                std::filesystem::path     qf  = rpp / szQuillFile;
                if(!std::filesystem::exists(qf)){
                    wkspDebug << "Quill fragment " << qf << " does not exist (not required to).";
                    continue;
                }
                
                QuillFile        sub;
                if(sub.load(qf) != std::error_code()){
                    wkspWarning << "Unable to read Quill fragment: " << qf;
                    continue;
                }
                
                
                if((!sub.abbr.empty()) && (depth < qs.abbr)){
                    abbr                = sub.abbr;
                    qs.abbr             = depth;
                }
                if((!sub.author.empty()) && (depth < qs.author)){
                    author              = sub.author;
                    qs.author           = depth;
                }
                
                if(sub.copyright.from && (depth < qs.copyfrom)){
                    copyright.from      = sub.copyright.from;
                    qs.copyfrom         = depth;
                }
                if((sub.copyright.stance != AssertDeny()) && (depth < qs.copystance)){
                    copyright.stance    = sub.copyright.stance;
                    qs.copystance       = depth;
                }
                if((!sub.copyright.text.empty()) && (depth < qs.copytext)){
                    copyright.text      = sub.copyright.text;
                    qs.copytext         = depth;
                }
                if(sub.copyright.to && (depth < qs.copyto)){
                    copyright.to        = sub.copyright.to;
                    qs.copyto           = depth;
                }
                if((!sub.home.empty()) && (depth < qs.home)){
                    home                = sub.home;
                    qs.home             = depth;
                }
                if((!sub.name.empty()) && (depth < qs.name)){
                    name                = sub.name;
                    qs.name             = depth;
                }
                
                load(sub, rSeen, pm2, tPolicy, false, depth+1, qs);
            }
        }
    }

    std::pair<RootDir*, bool>  Impl::make_root(const std::filesystem::path& s)
    {
        const RootDir*   r = rpath.get(s, nullptr);
        if(r)
            return { const_cast<RootDir*>(r), false };
        
        RootDir* r2        = new RootDir(s);
        r2 -> id        = root_dirs.size();
        root_dirs << r2;
        rpath[s]        = r2;
        return { r2, true };
    }
    
    path_vector_t           Impl::template_directories(const std::string_view& templateName)
    {
        path_vector_t   ret;
        for(const std::filesystem::path& p : template_dirs){
            std::filesystem::path    p2  = p / templateName;
            if(dir_type(p2) != BadDir)
                ret.push_back(p2);
        }
        return ret;
    }

    void                            set_db_init()
    {
        impl().db_init  = true;
    }

    
    //  ================================================================
    //     INFORMATION & QUERY -- valid AFTER successful initialize()
    //  ================================================================

    std::string_view                abbreviation()
    {
        return impl().abbr;
    }
    
    
    std::string_view                author()
    {
        return impl().author;
    }
    
    App                             app_type()
    {
        return impl().app;
    }
    
    const Set<uint16_t>&            aux_ports()
    {
        return impl().aux;
    }

    const std::filesystem::path&    cache()
    {
        return impl().cache;
    }
    
    bool                            can_cdb()
    {
        return impl().db_init;
    }

    const Copyright&                copyright()
    {
        return impl().copyright;
    }
    
    const std::filesystem::path&    dot()
    {
        return impl().dot;
    }

    const std::filesystem::path&    git()
    {
        return impl().git;
    }

    std::string_view                home()
    {
        return impl().home;
    }
    
    std::string_view                host()
    {
        return impl().host;
    }

    std::string_view                local_user()
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
    
    std::string_view                name()
    {
        return impl().name;
    }

    const Oracle&                   oracle()
    {
        return impl().oracle;
    }
    
    const std::filesystem::path&    perl()
    {
        return impl().perl;
    }
    
    uint16_t                        port()
    {
        return impl().port;
    }
    
    const std::filesystem::path&    quill_arg()
    {
        return impl().qspec;
    }

    const std::filesystem::path&    quill_dir()
    {
        return impl().qdir;
    }

    const std::filesystem::path&    quill_file()
    {
        return impl().qfile;
    }

    std::string_view                quill_key()
    {
        return impl().qkey;
    }


    std::filesystem::path           quill_resolve(const std::string_view&z)
    {
        return impl().qdir / z;
    }
    
    std::filesystem::path           quill_resolve(const std::filesystem::path&z)
    {
        return impl().qdir / z;
    }

    unsigned int                    read_timeout()
    {
        return impl().rtimeout;
    }
    
    path_vector_t                   resolve_all(const std::string_view& sv, bool templatesOnly)
    {
        path_vector_t       ret;
        for(const RootDir* r : impl().root_dirs){
            if(templatesOnly && !r->is_template)
                continue;
            std::filesystem::path   p   = r->resolve(sv);
            if(std::filesystem::exists(p))
                ret.push_back(p);
        }
        return ret;
    }
    
    const RootDir*                     root_dir(uint64_t j)
    {
        auto& i = impl();
        if(!j)
            return nullptr;
        if(j > i.root_dirs.size())
            return nullptr;
        return i.root_dirs[j-1];
    }
    
    const RootDir*                     root_dir(const std::filesystem::path&p)
    {
        return impl().rpath.get(p, nullptr);
    }
    
    const RootDir*                     root_dir(std::string_view k)
    {
        return impl().rkey.get(k, nullptr);
    }

    uint64_t                        root_count()
    {
        return impl().root_dirs.size();
    }
    
    const RootDir*                     root_first(DataRole dr)
    {
        return impl().rfirst[dr];
    }
    
    const root_role_map_t&          root_firsts()
    {
        return impl().rfirst;
    }
    
    const root_path_map_t&          root_path_map()
    {
        return impl().rpath;
    }
    
    const root_role_vec_map_t&      root_reads()
    {
        return impl().rread;
    }
    
    const root_vector_t&            root_reads(DataRole dr)
    {
        return impl().rread[dr];
    }
    
    const root_role_vec_map_t&      root_writes()
    {
        return impl().rwrite;
    }
    
    const root_vector_t&            root_writes(DataRole dr)
    {
        return impl().rwrite[dr];
    }
    
    const root_vector_t&            root_dirs()
    {
        return impl().root_dirs;
    }


    std::filesystem::path           shared(const std::string_view&sv)
    {
        for(auto& sd : impl().shared_dirs){
            auto p = sd / sv;
            if(file_exists(p))
                return p;
        }
        
        return std::filesystem::path();
    }
    
    std::filesystem::path           shared(const std::filesystem::path&sv)
    {
        for(auto& sd : impl().shared_dirs){
            auto p = sd / sv;
            if(file_exists(p))
                return p;
        }
        
        return std::filesystem::path();
    }

    path_vector_t                   shared_all(const std::string_view&sv)
    {
        path_vector_t       ret;
        for(auto& sd : impl().shared_dirs){
            auto p = sd / sv;
            if(file_exists(p))
                ret.push_back(p);
        }
        return ret;
    }
    
    path_vector_t                   shared_all(const std::filesystem::path&sv)
    {
        path_vector_t       ret;
        for(auto& sd : impl().shared_dirs){
            auto p = sd / sv;
            if(file_exists(p))
                ret.push_back(p);
        }
        return ret;
    }

    const path_vector_t&            shared_dirs()
    {
        return impl().shared_dirs;
    }
    
    const std::filesystem::path&    smartypants()
    {
        return impl().smartypants;
    }
    
    std::string_view                start()
    {
        return impl().start_gui;
    }

    std::string_view                start_file()
    {
        return impl().start_file;
    }
    
    std::time_t                     start_time()
    {
        return impl().start;
    }
    
    const std::filesystem::path&    subversion()
    {
        return impl().subversion;
    }
    
    const std::filesystem::path&    temp_dir()
    {
        return impl().tmp;
    }

    std::filesystem::path           temp_resolve(const std::string_view&z)
    {
        return impl().tmp / z;
    }
    
    std::filesystem::path           temp_resolve(const std::filesystem::path&z)
    {
        return impl().tmp / z;
    }

    
    const path_vector_t&            template_dirs()
    {
        return impl().template_dirs;
    }
    
    path_vector_t                   template_dirs(const std::string_view& templateName)
    {
        path_vector_t  ret;
        for(const std::filesystem::path& p : impl().template_dirs){
            std::filesystem::path    p2  = p / templateName;
            if(dir_type(p2) != BadDir)
                ret.push_back(p2);
        }
        return ret;
    }
    
    std::filesystem::path           template_quill(const std::string_view& templateName)
    {
        for(const std::filesystem::path& p : template_dirs(templateName)){
            std::filesystem::path p2 = p / szQuillFile;
            if(std::filesystem::exists(p2))
                return p2;
        }
        return std::filesystem::path();
    }

    const string_set_t&             templates(bool all)
    {
        return all ? impl().available : impl().templates;
    }
    
    unsigned int                    threads()
    {
        return impl().threads;
    }
}

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace yq::mithril {
    const RootDir*  root_dir(Root rt)
    {
        return RootDir::by_id(rt.id);
    }

    const RootDir*  RootDir::by_id(uint64_t u)
    {
        return wksp::root_dir(u);
    }

    const RootDir*  RootDir::by_key(const std::string_view&k)
    {
        return wksp::root_dir(k);
    }

    RootDir::RootDir(const std::filesystem::path&s) : path(s)
    {
        if(dir::child_exists(s, ".svn"))
            vcs |= Vcs::SVN;
        if(dir::child_exists(s, ".git"))
            vcs |= Vcs::GIT;
    }

    RootDir::RootDir(const std::filesystem::path&s, PolicyMap pm) : RootDir(s)
    {
        access  = pm;
    }


    RootDir::~RootDir()
    {
    }

    bool        RootDir::has_git() const
    {
        return vcs.is_set(Vcs::GIT);
    }

    bool        RootDir::has_subversion() const
    {
        return vcs.is_set(Vcs::SVN);
    }

    bool        RootDir::is_good(DataRole dr, Access ac) const
    {
        switch(ac){
        case Access::ReadOnly:
            return is_readable(dr);
        case Access::ReadWrite:
        case Access::WriteFirst:
            return is_writable(dr);
        case Access::NoAccess:
        case Access::Default:
        default:
            return true;
        }
    }

    bool        RootDir::is_readable(DataRole dr) const
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

    bool        RootDir::is_writable(DataRole dr) const
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

    bool        RootDir::is_write_first(DataRole dr) const
    {
        return policy(dr) == Access::WriteFirst;
    }


    Access      RootDir::policy(DataRole dr) const
    {
        return access.get(dr);
    }

    std::filesystem::path   RootDir::resolve(const std::filesystem::path& z) const
    {
        return path / z;
    }
    
    namespace {
        YQ_INVOKE(
            writer<Global>().variable("abbreviation", string_view_proxy<wksp::abbreviation>);
            writer<Global>().variable("host", string_view_proxy<wksp::host>);
            writer<Global>().variable("name", string_view_proxy<wksp::name>);
            writer<Global>().variable("port", wksp::port);
            writer<Global>().variable("start", string_view_proxy<wksp::start>);
            writer<Global>().variable("threads", wksp::threads);
        );
    }

}
