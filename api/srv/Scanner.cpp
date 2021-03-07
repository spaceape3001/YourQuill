////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Importer.hpp"
#include "Scanner.hpp"

#include <db/FileSys.hpp>
#include <db/Root.hpp>
#include <db/Workspace.hpp>
//#include <util/EnumMap.hpp>
#include <util/FileUtils.hpp>
#include <util/Logging.hpp>
#include <util/SqlQuery.hpp>
//#include "util/Set.hpp"
//#include "util/Utilities.hpp"
//#include "util/Vector.hpp"

#include <QCoreApplication>
#include <QDateTime>
#include <QFileInfo>
#include <QTimer>


#include <QRegularExpressionMatch>


#define sAlert           yAlert("scanner")
#define sCritical        yCritical("scanner")
#define sDebug           yDebug("scanner")
#define sError           yError("scanner")
#define sEmergency       yEmergency("scanner")
#define sFatal           yFatal("scanner")
#define sInfo            yInfo("scanner")
#define sNotice          yNotice("scanner")
#define sWarning         yWarning("scanner")


static constexpr const bool         kScanMetrics        = false;
static constexpr const unsigned int kScanTickInterval   = 100;
static const StringSet              kIgnore({ 
    ".bench", 
    ".cache", 
    ".git", 
    ".logs", 
    ".quill",
    ".server.ini",
    ".svn", 
    ".users"
});

struct Scanner::D {
    const Root*     root;
    String          dpath;
    QStringList     path;
    QString         key;
    bool            isRoot;
    Folder          folder;
    Directory       directory;
    
    D() : root(nullptr), isRoot(false) {}
    D(const Root* d) : root(d), isRoot(true)
    {
        dpath       = d->path();
        folder      = cdb::top_folder();
        directory   = cdb::db_directory(d);
    }
    
    //D(const Root* _r, const QString& s) : root(_r), dir(s){}
    D(const D& d, const String & s) : 
        root(d.root), 
        path(d.path), isRoot(false)
    {
        QString     qs = s.qString();
        dpath       = d.dpath + "/" + s;
        folder      = cdb::db_folder(d.folder, qs); // const_cast<D&>(d).folder.create_child(qs);
        directory   = cdb::db_directory(d.directory, qs); //  const_cast<D&>(d).directory.create_child(qs);
        path << qs;
        if(!d.isRoot)
            key += '/';
        key += qs;
    }
    
    Vector<String>          fragments() const
    {
        return dir_files(dpath.c_str());
    }
    
    Vector<String>          directories() const
    {
        return dir_directories(dpath.c_str());
    }
    
    Map<QString,Directory>  child_dirs() const
    {
        Map<QString,Directory>  ret;
        static thread_local SqlQuery    s(wksp::cache(), "SELECT id,name FROM Directories WHERE parent=? AND removed=0");
        auto s_af = s.af();
        s.bind(0, directory.id);
        if(s.exec()){
            while(s.next())
                ret[s.valueString(1)]   = Directory(s.valueU64(0));
        }
        return ret;
    }
    
    Map<String,F>           db_fragments() const;
    

    D(const D&) = default;
    D(D&&) = default;
    ~D() = default;
    D& operator=(const D&) = default;
    D& operator=(D&&) = default;
};

struct Scanner::F {
    Change          chg;
    Fragment        fragment;
    String          path;
    uint64_t        size;
    uint64_t        modified;
    bool            rescan, exist;
    
    F() : size(0), modified(0), rescan(false), exist(false) {}
    void    update(uint64_t sz, uint64_t ns)
    {
        static thread_local SqlQuery    u(wksp::cache(), "UPDATE Fragments SET bytes=?,modified=?,rescan=0 WHERE id=?");
        auto u_af = u.af();
        u.bind(0,sz);
        u.bind(1,ns);
        u.bind(2, fragment.id);
        u.exec();
        size        = sz;
        modified    = ns;
        rescan      = false;
    }
    
    void    remove()
    {
        static thread_local SqlQuery u(wksp::cache(), "UPDATE Fragments SET removed=1 WHERE id=?");
        auto u_af = u.af();
        u.bind(0, fragment.id);
        u.exec();
        exist       = false;
    }
};


struct Scanner::W {
    QString         fragment;
    QDateTime       mod;
    void        (*fn)();
    
    W() : fn(nullptr) {}
    W(const QString& f, void(*chg)()) : fragment(f), fn(chg) {
        mod     = QFileInfo(fragment).lastModified();
    }
};

struct Scanner::I {
    Vector<W>       watch;
    StringSet       ignore;
    
    I() : ignore({
            ".cache", 
            ".git", 
            ".logs", 
            ".svn", 
            ".yquill",
            ".users"
        })
    {
    }
};

Scanner::I& Scanner::sImpl() 
{
    static I*       ret = new I;
    return *ret;
}

void        Scanner::checkWatch()
{
    I&  s   = sImpl();
    for(W& e : s.watch){
        QDateTime       dt  = QFileInfo(e.fragment).lastModified();
        if(dt != e.mod){
            e.fn();
            e.mod   = dt;
        }
    }
}

void        Scanner::watch(const QString&f, void(*ch)())
{
    I&  s   = sImpl();
    s.watch << W(f, ch);
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Scanner::Scanner() : m_timer(nullptr), m_primed(false), m_first(true), m_time(0.)
{
    moveToThread(this);
}

Scanner::~Scanner()
{
}

void    Scanner::prime()
{
    if(!m_primed)
        scan();
}


//Scanner::D  Scanner::next()
//{
    //if(m_scan.empty())
        //return D();
    //D   ret = m_scan.front();
    //m_scan.pop_front();
    //for(QString s : ret.dir.entryList(QDir::Dirs | QDir::AllDirs | QDir::NoDotAndDotDot |  QDir::Readable | QDir::Executable | QDir::Hidden))
        //m_scan << D(ret, s);
    //return ret;
//}

void        Scanner::run()
{
    prime();
    m_timer     = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Scanner::timeout);
    m_timer -> setInterval(kScanTickInterval);
    m_timer -> start();
    exec();
    m_timer -> stop();
}

void        Scanner::timeout()
{
    checkWatch();
    scan();
}

Map<String,Scanner::F>  Scanner::D::db_fragments() const
{
    Map<String,Scanner::F>         ret;
    static thread_local SqlQuery    s(wksp::cache(), "SELECT id,name,rescan,modified,bytes,path FROM Fragments WHERE dir=? AND removed=0");
    auto s_af   = s.af();
    s.bind(0, directory.id);
    if(s.exec()){
        while(s.next()){
            F&  f       = ret[s.valueString(1)];
            f.path      = s.valueString(5);
            f.chg       = Change::Removed;
            f.fragment  = Fragment(s.valueU64(0));
            f.size      = s.valueU64(4);
            f.modified  = s.valueU64(3);
            f.rescan    = s.valueAs<bool>(2);
            f.exist     = true;
        }
    }
    return ret;
}

void        Scanner::scan()
{
    //static const auto                       s_importers = Importer::repo().byChange;
    static const Vector<const Root*>&       s_roots     = wksp::roots();
    static I&           si  = sImpl();
    
    if(s_roots.empty()){  // nothing will *EVER* happen if this is the case
        sNotice << "No root directories, no point, stopping/exiting the fragment scanner thread.";
        m_timer -> stop();
        quit();
        return ;
    }
    
    auto            start   = std::chrono::steady_clock::now();
    auto            end     = start + std::chrono::milliseconds( (9*kScanTickInterval)/10 );   // go for 90% duty cycle, maximum....
    bool            notime  = !m_primed;
    
    if(m_scan.empty()){
        if(m_primed){
            if(m_first){
                m_first         = false;
            } else if(kScanMetrics){
                sInfo << "Scanner found " << m_nFragments << " fragments in " <<  m_nIter << " (" << kScanTickInterval 
                    << "ms) intervals using " << m_time << " milliseconds of CPU time.";

                m_time          = 0.;
                m_nIter         = 0;
                m_nFragments    = 0;
            }
        }
        
        for(const Root* r : s_roots)
            m_scan << D(r);
    }
    
    //static thread_local SqlQuery    sRemove(wksp::cache(), "DELETE FROM Fragments WHERE id=?");

    //auto x_lk   = sExisting.af();
    //auto r_lk   = sRemove.af();

    Vector<Directory>       missing_dirs;
    auto& slist = Importer::change_map();

    start   = std::chrono::steady_clock::now();
    while((!m_scan.empty()) && (notime || (std::chrono::steady_clock::now() < end))){
    
        D   d = m_scan.pop();
        
        Map<QString,Directory>  missing     = d.child_dirs();
        for(String s : d.directories()){
            if(d.isRoot && si.ignore.has(s))
                continue;
            missing.erase(s.qString());
            m_scan << D(d, s);
        }
        
        missing_dirs += missing.values();

        Map<String,F>       fragments = d.db_fragments();
        for(String s : d.fragments()){
                // prune out temporaries....
            if(s.ends_with("~"))
                continue;
            if(s.ends_with("-"))
                continue;
            if(s.ends_with(".bck"))
                continue;
                
            if(d.isRoot && si.ignore.has(s))
                continue;
            if(kScanMetrics && !notime)
                ++m_nFragments;
            
            F&  f   = fragments[s];
            if(!f.exist){
                f.path       = d.dpath + "/" + s;
                f.fragment   = cdb::db_fragment(d.directory, s.qString());
                f.chg        = Change::Added;
            } 
            
            SizeTimestamp   sz  = file_size_and_timestamp(f.path);
            uint64_t        ns  = sz.nanoseconds();

            if(!m_primed){
                f.update(sz.size, ns);
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
                for(const Importer* i : slist[k.second.chg]){
                    if(!i->matches(d.key, k.first.qString()))
                        continue;
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


#include "moc_Scanner.cpp"
