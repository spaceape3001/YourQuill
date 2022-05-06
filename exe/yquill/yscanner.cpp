////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yquill.hpp"

#include <db/filesys.hpp>
#include <db/wksp/workspace.hpp>

#include <yq/algo/Compare.hpp>
#include <yq/collection/Deque.hpp>
#include <yq/collection/EnumMap.hpp>
#include <yq/collection/Set.hpp>
#include <yq/collection/Vector.hpp>
#include <yq/io/dir_utils.hpp>
#include <yq/io/DirWatcher.hpp>
#include <yq/io/file_utils.hpp>
#include <yq/log/Logging.hpp>
#include <yq/sql/SqlQuery.hpp>
#include <yq/srv/Notifier.hpp>
#include <yq/srv/Stage2.hpp>
#include <yq/srv/Stage3.hpp>
#include <yq/srv/Stage4.hpp>
#include <yq/text/text_utils.hpp>


#include <chrono>
#include <condition_variable>
#include <mutex>

using namespace yq;

static constexpr const bool         kScanMetrics        = false;
static constexpr const unsigned int kScanTickInterval   = 100;


#define sAlert           yAlert("scanner")
#define sCritical        yCritical("scanner")
#define sDebug           yDebug("scanner")
#define sError           yError("scanner")
#define sEmergency       yEmergency("scanner")
#define sFatal           yFatal("scanner")
#define sInfo            yInfo("scanner")
#define sNotice          yNotice("scanner")
#define sWarning         yWarning("scanner")

struct RootPath {
    const yq::Root*             root    = nullptr;
    std::filesystem::path       path;       //!< Relative path if root present, absolute otherwise
};

RootPath    rootPath(const std::filesystem::path&fp)
{
    static const std::vector<const Root*>& roots = wksp::roots();
    
    for(const Root* r : roots){
        std::error_code         ec;
        std::filesystem::path   p   = std::filesystem::relative(fp, r->path, ec);
        if(ec != std::error_code())
            continue;
        if(starts(p.native(), "../"))
            continue;
        if(starts(p.native(), "/"))
            continue;
        return RootPath{ r, p };
    }
    
    return RootPath{ nullptr, fp };
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

struct DQ1 {
    Folder                  folder;
    Directory               directory;
    std::filesystem::path   path;
};

std::string_view    suffix(std::string_view n)
{
    size_t  i   = n.find_first_of('.', 1);
    if(i != std::string_view::npos)
        return n.substr(i);
    return std::string_view();
}

bool    ignore(Folder f, std::string_view n)
{
    // weed out bad directories to follow
    if(n == ".")
        return true;
    if(n == "..")
        return true;
    if(n == ". ")   // Don't know how this got onto the drive in the first place...
        return true;
        
    if(f == cdb::top_folder()){
        // weed out reserved files
        if(is_similar(n, ".cache"))
            return true;
        if(is_similar(n, ".git"))
            return true;
        if(is_similar(n, ".logs"))
            return true;
        if(is_similar(n, ".quill"))
            return true;
        if(is_similar(n, ".svn"))
            return true;
        if(is_similar(n, ".users"))
            return true;
        if(is_similar(n, ".yquill"))
            return true;
    }

        // prune out temporaries....
    if(starts(n, ".#"))
        return true;
    if(ends(n, "~"))
        return true;
    if(ends(n, "-"))
        return true;
    if(ends(n, ".bck"))
        return true;
    if(ends(n, ".disable"))
        return true;
    if(ends(n, ".bad"))
        return true;
    if(ends(n, ".orig"))
        return true;
    if(starts(suffix(n), ".draft"))
        return true;

    return false;
}


        //  Stage 1 -- only scans, creates the directories/fragments in the database
void    stage1_scan()
{
    Deque<DQ1>      queue;
    
    for(const Root* rt : wksp::roots())
        queue << DQ1{cdb::top_folder(), cdb::db_root(rt), rt->path};
    
    while(!queue.empty()){
        DQ1     dq;
        queue >> dq;
        
        dir::for_all_children(dq.path, dir::HIDDEN, [&](const std::filesystem::path& p){
            auto n  = p.filename().string();
            if(ignore(dq.folder, n))
                return;
                
            if(std::filesystem::is_directory(p)){
                DQ1 dq2;
                std::tie(dq2.directory, dq2.folder)    = cdb::db_directory(dq.directory, n);
                if(dq2.directory){
                    dq2.path   = p;
                    queue << dq2;
                }
            } else {
                Fragment frag = cdb::db_fragment(dq.directory, n).first;
                if(frag)
                    cdb::update(frag);
            }
        });
    }
}

        //  Stage 2 -- call the on_startups with zero arguments
void    stage2_unimport()
{
    Vector<const Stage2*>   handlers    = Stage2::all();
    std::stable_sort(handlers.begin(), handlers.end(), [](const Stage2*a, const Stage2*b) -> bool {
        return a->order() < b->order();
    });
    for(const Stage2* h : handlers)
        h->invoke();
}

        //  Stage 3 -- call the on_startup for all the files with respective importing
void    stage3_documents()
{
    Vector<const Stage3*>   handlers    = Stage3::all();
    std::stable_sort(handlers.begin(), handlers.end(), [](const Stage3*a, const Stage3*b) -> bool {
        return a->order() < b->order();
    });
    
    SqlQuery                sn(wksp::db(), "SELECT id FROM Documents WHERE name=? ORDER BY k");
    SqlQuery                sx(wksp::db(), "SELECT id FROM Documents WHERE suffix=? ORDER BY k");
    SqlQuery                sfn(wksp::db(), "SELECT id FROM Documents WHERE folder=? AND name=? ORDER BY k");
    SqlQuery                sfx(wksp::db(), "SELECT id FROM Documents WHERE folder=? AND suffix=? ORDER BY k");
    
    for(const Stage3* h : handlers){
        SqlQuery::AutoFinish    af;
        switch(h->method()){
        case Stage3::ByName:
            af  = sn.autoFinish();
            sn.bind(1, h->specific());
            while(sn.step() == SqlQuery::Row){
                h -> invoke(Document{ sn.v_uint64(1)});
            }
            break;
        case Stage3::ByExt:
            af  = sx.autoFinish();
            sx.bind(1, h->specific());
            while(sx.step() == SqlQuery::Row)
                h -> invoke(Document{ sx.v_uint64(1)});
            break;
        case Stage3::ByFolderName:
            af  = sfn.autoFinish();
            sfn.bind(1, h->folder().id);
            sfn.bind(2, h->specific());
            while(sfn.step() == SqlQuery::Row)
                h -> invoke(Document{ sfn.v_uint64(1)});
            break;
        case Stage3::ByFolderExt:
            af  = sfx.autoFinish();
            sfx.bind(1, h->folder().id);
            sfx.bind(2, h->specific());
            while(sfx.step() == SqlQuery::Row)
                h -> invoke(Document{ sfx.v_uint64(1)});
            break;
        default:
            break;
        }
    }
}

        // Stage 4 -- pass after stage 3
void    stage4_finalize()
{
    Vector<const Stage4*>   handlers    = Stage4::all();
    std::stable_sort(handlers.begin(), handlers.end(), [](const Stage4*a, const Stage4*b) -> bool {
        return a->order() < b->order();
    });
    for(const Stage4* h : handlers)
        h->invoke();
}

std::string_view    filename(std::string_view fn)
{
    size_t  n = fn.find_last_of('/');
    if(n==0)
        return fn.substr(1);
    if(n == std::string_view::npos)
        return fn;
    return fn.substr(n+1);
}

EnumMap<Change,Vector<const Notifier*>> change_map()
{
    EnumMap<Change,Vector<const Notifier*>> ret = Notifier::change_map();
    for(Change c : Change::all_values()){
        ret[c].stable_sort([](const Notifier*a, const Notifier*b) -> bool {
            return a->order() < b->order();
        });
    }
    return ret;
}


void    dispatch(Change change, Fragment frag, Folder fo, const std::filesystem::path& p)
{
    static EnumMap<Change,Vector<const Notifier*>> changeMap = change_map();

    std::string_view    name    = filename(p.native());
    std::string_view    ext     = file_extension(name);

    for(const Notifier* n : changeMap[change]){
        switch(n->trigger()){
        case Notifier::ByFile:
            if(p != n->path())
                continue;
            break;
        case Notifier::ByFolderDoc:
            if(fo != n->folder())
                continue;
            // fallthrough is deliberate
        case Notifier::ByDocument:
            if(!is_similar(name, n->specific()))
                continue;
            break;
        case Notifier::ByFolderExt:
            if(fo != n->folder())
                continue;
            // fallthrough is deliberate
        case Notifier::ByExtension:
            if(!is_similar(ext, n->specific()))
                continue;
            break;
        default:
            continue;
        }
        
        n -> change(frag, change);
    }
    
    cdb::update(frag);
}


void    notify(Change change, Fragment frag, Folder fo, const std::filesystem::path& p=std::filesystem::path())
{
    dispatch(change, frag?frag:cdb::fragment(p), fo?fo:cdb::folder(frag), p.empty()?cdb::path(frag):p);
}


void    check_file(Folder folder, Fragment frag, const std::filesystem::path& p)
{
    auto info           = cdb::info(frag);
    SizeTimestamp       szt = file_size_and_timestamp(p);
    
    if((szt.nanoseconds() == info.modified) && (szt.size == info.size) && (szt.exists == !info.removed))
        return ;
        
    if(szt.exists && info.removed){
        notify(Change::Added, frag, folder, p);
    } else if(!info.removed && !szt.exists){
        notify(Change::Removed, frag, folder, p);
        cdb::erase(frag);
    } else {
        notify(Change::Modified, frag, folder, p);
    }
}

void    dir_removed(Directory d, Folder fo=Folder())
{
    if(!fo)
        fo  = cdb::folder(d);
    for(Fragment f : cdb::child_fragments(d)){
        notify(Change::Removed, f, fo);
        cdb::erase(f);
    }
    for(Directory d2 : cdb::child_directories(d))
        dir_removed(d2);
    cdb::erase(d);
}

        //  Stage 5 -- scan/sweep for changes
void    stage5_sweep()
{
    Deque<DQ1>      queue;
    
    for(const Root* rt : wksp::roots())
        queue << DQ1{cdb::top_folder(), cdb::db_root(rt), rt->path};

    while(!queue.empty()){
        DQ1     dq;
        queue >> dq;
        
        // so we can process removes
        auto frags  = cdb::child_fragments_set(dq.directory);
        auto dirs   = cdb::child_directories_set(dq.directory);
        
        dir::for_all_children(dq.path, dir::HIDDEN, [&](const std::filesystem::path& p){
            auto n  = p.filename().string();
            if(ignore(dq.folder, n))
                return;
            
            //  TODO.... not copy & paste
            if(std::filesystem::is_directory(p)){
                DQ1 dq2;
                std::tie(dq2.directory, dq2.folder)    = cdb::db_directory(dq.directory, n);
                if(dq2.directory){
                    dq2.path   = p;
                    queue << dq2;
                    dirs.erase(dq2.directory);
                }
            } else {
                bool    fWasCreate  = false;
                Fragment    frag;
                Document    doc;
                std::tie(frag, doc) = cdb::db_fragment(dq.directory, n, &fWasCreate);
                
                if(fWasCreate){
                    notify(Change::Added, frag, dq.folder, p);
                } else {
                    check_file(dq.folder, frag, p);
                }
                frags.erase(frag);
            }
        });
        
        for(Directory d2 : dirs)
            dir_removed(d2, dq.folder);
        for(Fragment f2 : frags){
            notify(Change::Removed, f2, dq.folder);
            cdb::erase(f2);
        }
        

        if(gQuit != Quit::No)
            break;
    }
}



//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#if 0
struct YScanner::D {
    const Root*         root;
    std::string         dpath;
    Vector<std::string> path;
    std::string         key;
    bool                isRoot = false;
    Folder              folder;
    Directory           directory;
    
    D() : root(nullptr), isRoot(false) {}
    D(const Root* d) : root(d), isRoot(true)
    {
        dpath       = d->path;
        folder      = cdb::top_folder();
        directory   = cdb::db_directory(d);
    }
    
    //D(const Root* _r, const QString& s) : root(_r), dir(s){}
    D(const D& d, const std::string& s) : 
        root(d.root), 
        path(d.path), isRoot(false)
    {
        dpath       = d.dpath + "/" + s;
        folder      = cdb::db_folder(d.folder, s); // const_cast<D&>(d).folder.create_child(qs);
        directory   = cdb::db_directory(d.directory, s); //  const_cast<D&>(d).directory.create_child(qs);
        path << s;
        if(!d.isRoot)
            key += '/';
        key += s;
    }
    
    Vector<std::string>    fragments() const
    {
        return dir_files(dpath.c_str());
    }
    
    Vector<std::string>    directories() const
    {
        return dir_directories(dpath.c_str());
    }
    
    Map<std::string,Directory>  child_dirs() const
    {
        Map<std::string,Directory>  ret;
        static thread_local SqlQuery    s(wksp::db(), "SELECT id,name FROM Directories WHERE parent=? AND removed=0");
        auto s_af = s.af();
        s.bind(1, directory.id);
        while(s.step() == SqlQuery::Row){
            ret[copy(s.v_text(2))]   = Directory(s.v_uint64(1));
        }
        return ret;
    }
    
    Map<std::string,F>           db_fragments() const;
    

    D(const D&) = default;
    D(D&&) = default;
    ~D() = default;
    D& operator=(const D&) = default;
    D& operator=(D&&) = default;
};

struct YScanner::F {
    Change          chg;
    Fragment        fragment;
    std::string     path;
    uint64_t        size        = 0;
    uint64_t        modified    = 0;
    bool            rescan = false, exist = false;
    
    void    update(uint64_t sz, uint64_t ns)
    {
        static thread_local SqlQuery    u(wksp::db(), "UPDATE Fragments SET bytes=?,modified=?,rescan=0 WHERE id=?");
        auto u_af = u.af();
        u.bind(1,sz);
        u.bind(2,ns);
        u.bind(3, fragment.id);
        u.step();
        size        = sz;
        modified    = ns;
        rescan      = false;
    }
    
    void    remove()
    {
        static thread_local SqlQuery u(wksp::db(), "UPDATE Fragments SET removed=1 WHERE id=?");
        auto u_af = u.af();
        u.bind(1, fragment.id);
        u.step();
        exist       = false;
    }
};


struct YScanner::W {
    const Notifier* notify  = nullptr;
    SizeTimestamp   szt;
};

//struct Scanner::T {
    
//};



Map<std::string,YScanner::F>  YScanner::D::db_fragments() const
{
    Map<std::string,YScanner::F>         ret;
    static thread_local SqlQuery    s(wksp::db(), "SELECT id,name,rescan,modified,bytes,path FROM Fragments WHERE dir=? AND removed=0");
    auto s_af   = s.af();
    s.bind(1, directory.id);
    while(s.step() == SqlQuery::Row){
        F&  f       = ret[copy(s.v_text(2))];
        f.fragment  = Fragment(s.v_uint64(1));
        f.rescan    = s.v_bool(3);
        f.modified  = s.v_uint64(4);
        f.size      = s.v_uint64(5);
        f.path      = s.v_text(6);
        f.chg       = Change::Removed;
        f.exist     = true;
    }
    return ret;
}
//  --------------------------------------------------------------------------------------------------------------------


YScanner::YScanner()
{
    m_ignore 
        << "." << ".." 
        << ".cache" 
        << ".git" 
        << ".logs" 
        << ".quill" 
        << ".svn" 
    ;
    
    for(const Notifier* n : Notifier::all()){
        if(n && (n->trigger() == Notifier::ByFile))
            m_watch << W{n};
    }
}

YScanner::~YScanner()
{
}

void    YScanner::do_work()
{
    if(m_bad)
        return ;

    checkWatch();
    scan();
}

void     YScanner::checkWatch()
{
    for(W& w : m_watch){
        SizeTimestamp   szt = file_size_and_timestamp(w.notify->path());
        if(szt != w.szt){
            w.szt   = szt;
            w.notify->change(Fragment(), Change::Modified);
        }
    }
}


void YScanner::prime()
{
    if(!m_primed){
        checkWatch();
        scan();
    }
}

void     YScanner::scan()
{
    static const Vector<const Root*>&       s_roots     = wksp::roots();
    //static I&           si  = sImpl();
    
    if(s_roots.empty()){  // nothing will *EVER* happen if this is the case
        m_bad   = true;
        sNotice << "No root directories, no point, stopping/exiting the fragment scanner thread.";
        return;
    }
    
    auto            start   = std::chrono::steady_clock::now();
    auto            end     = start + std::chrono::milliseconds( (9*kScanTickInterval)/10 );   // go for 90% duty cycle, maximum....
    bool            notime  = !m_primed;
    
    if(m_scan.empty()){
        if(m_primed){
            if(m_first){
                m_first         = false;
                m_startup       = true;
            } else {
                m_startup       = false;
                if(kScanMetrics){
                    sInfo << "Scanner found " << m_nFragments << " fragments in " <<  m_nIter << " (" << kScanTickInterval 
                        << "ms) intervals using " << m_time << " milliseconds of CPU time.";

                    m_time          = 0.;
                    m_nIter         = 0;
                    m_nFragments    = 0;
                }
            }
        }
        
        for(const Root* r : s_roots)
            m_scan << D(r);
    }
    
    //static thread_local SqlQuery    sRemove(wksp::cache(), "DELETE FROM Fragments WHERE id=?");

    //auto x_lk   = sExisting.af();
    //auto r_lk   = sRemove.af();

    Vector<Directory>       missing_dirs;
    auto& slist = Notifier::change_map();

    start   = std::chrono::steady_clock::now();
    while((!m_scan.empty()) && (notime || (std::chrono::steady_clock::now() < end))){
    
        D   d = m_scan.pop();
        
        Map<std::string,Directory>  missing     = d.child_dirs();
        for(auto& s : d.directories()){
            if(d.isRoot && m_ignore.has(s))
                continue;
            missing.erase(s);
            m_scan << D(d, s);
        }
        
        missing_dirs += missing.values();

        Map<std::string,F>       fragments = d.db_fragments();
        for(const std::string& s : d.fragments()){
                // prune out temporaries....
            if(ends(s, "~"))
                continue;
            if(ends(s, "-"))
                continue;
            if(ends(s, ".bck"))
                continue;
                
            if(d.isRoot && m_ignore.has(s))
                continue;
            if(kScanMetrics && !notime)
                ++m_nFragments;
            
            F&  f   = fragments[s];
            if(!f.exist){
                f.path       = d.dpath + "/" + s;
                f.fragment   = cdb::db_fragment(d.directory, s);
                f.chg        = Change::Added;
            } 
            
            SizeTimestamp   sz  = file_size_and_timestamp(f.path);
            uint64_t        ns  = sz.nanoseconds();

            if(!m_primed){
                f.update(sz.size, ns);
                continue;
            }
            
            if(m_startup){
                f.chg           = Change::Startup;
                continue;
            }
            
            if(f.rescan || (f.modified != ns) || (f.size != sz.size)){
                //yDebug() << "File " << f.path << " modified";            
                f.chg           = Change::Modified;
                f.update(sz.size, ns);
            } else 
                f.chg           = Change::None;
        }

        if(m_primed){
        
            //  Check fragments.... 
            for(auto& k : fragments){
                for(const Notifier* i : slist[k.second.chg]){
                    switch(i->trigger()){
                    case Notifier::ByFolderDoc:
                        if(cdb::folder(k.second.fragment) != i->folder())
                            continue;
                        // [[fallthrough]]
                    case Notifier::ByDocument:
                        if(!is_similar(k.first, i->specific()))
                            continue;
                        break;
                    case Notifier::ByFolderExt:
                        if(cdb::folder(k.second.fragment) != i->folder())
                            continue;
                        // [[fallthrough]]
                    case Notifier::ByExtension:
                        if(!is_similar(file_extension(k.first), i->specific()))
                            continue;
                        break;
                    case Notifier::NoTrigger:
                    default:
                        continue;
                    }
                
                    i->change( k.second.fragment, k.second.chg);
                }
                
                if(k.second.chg == Change::Removed)
                    k.second.remove();
            }
        }
    }
    
    //  Now, check missing directories....
    
    //  ... TODO ...

    if(kScanMetrics && !notime){
        m_time += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start).count() * 1e-6;
        ++m_nIter;
    }
    
    m_primed    = true;
}
#endif


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void        run_scanner(Vector<std::thread>& threads)
{
    std::condition_variable cv;
    std::mutex              mutex;
    bool                    ready = false;
    
    threads << std::thread([&](){
        thread::id();

        ready       = true;
        cv.notify_one();

        while(gQuit == Quit::No){
            std::this_thread::sleep_for(10ms);
            stage5_sweep();
        }
    });
    
    {
        std::unique_lock lk(mutex);
        cv.wait(lk, [&]{ return ready; });
    }
}

