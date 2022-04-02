////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yquill.hpp"
#include "yscanner.hpp"

#include <yq/enum/Change.hpp>
#include <yq/file/cdb.hpp>
#include <yq/file/FileUtils.hpp>
#include <yq/file/Notifier.hpp>
#include <yq/log/Logging.hpp>
#include <yq/sql/SqlQuery.hpp>
#include <yq/text/Utils.hpp>
#include <yq/wksp/Root.hpp>
#include <yq/wksp/Workspace.hpp>


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
        << ".users"
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


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void        run_scanner(Vector<std::thread>& threads)
{
    using namespace std::literals::chrono_literals;
    std::condition_variable cv;
    std::mutex              mutex;
    bool                    ready = false;
    
    threads << std::thread([&](){
        thread::id();

        YScanner scan;
        scan.prime();

        ready       = true;
        cv.notify_one();

        while(gQuit == Quit::No){
            std::this_thread::sleep_for(1ms);
            scan.do_work();
            if(scan.is_bad())
                break;
        }
    });
    
    {
        std::unique_lock lk(mutex);
        cv.wait(lk, [&]{ return ready; });
    }
}

