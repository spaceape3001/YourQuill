////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "config.hpp"
#include "yquill.hpp"

#include <yq/core/Compare.hpp>
#include <yq/core/Logging.hpp>
#include <yq/core/ThreadId.hpp>
#include <yq/container/Deque.hpp>
#include <yq/container/EnumMap.hpp>
#include <yq/container/Set.hpp>
#include <yq/container/Vector.hpp>
#include <yq/file/DirUtils.hpp>
#include <yq/file/FileUtils.hpp>
#include <yq/sql/SqlLite.hpp>
#include <yq/sql/SqlQuery.hpp>
#include <yq/text/chars.hpp>
#include <yq/text/match.hpp>


#include <mithril/directory/DirectoryCDB.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/root/RootCDB.hpp>
#include <mithril/ipc/DirWatcher.hpp>
#include <mithril/notify/FileWatch.hpp>
#include <mithril/notify/FileNotifier.hpp>
#include <mithril/notify/Stage2.hpp>
#include <mithril/notify/Stage3.hpp>
#include <mithril/notify/Stage4.hpp>
#include <mithril/notify/Stage5.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <mithril/wksp/Workspace.hpp>


#include <chrono>
#include <condition_variable>
#include <mutex>

using namespace yq;
using namespace yq::mithril;
using namespace std::literals::chrono_literals;

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
    const yq::mithril::RootDir*    root_dir    = nullptr;
    std::filesystem::path       path;       //!< Relative path if root_dir present, absolute otherwise
};

RootPath    rootPath(const std::filesystem::path&fp)
{
    static const std::vector<const RootDir*>& roots = wksp::root_dirs();
    
    for(const RootDir* r : roots){
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

struct Watch {
    const FileWatch*    watcher     = nullptr;
    SizeTimestamp       szt;
};

std::vector<Watch>      gWatch;

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
    if(n == ". ")   // Don't know how this got onto the drive in the first place... still bad
        return true;
        
    if(f == cdb::top_folder()){
        // weed out reserved files
        if(is_in(n, config.topFolder_exclude))
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
    if(ends(n, ".tmp"))
        return true;
    if(ends(n, ".disable"))
        return true;
    if(ends(n, "-disable"))
        return true;
    if(ends(n, "-old"))
        return true;
    if(ends(n, ".old"))
        return true;
    if(ends(n, ".openicon"))
        return true;
    if(ends(n, ".svgrepo"))
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
    
    for(const RootDir* rt : wksp::root_dirs())
        queue << DQ1{cdb::top_folder(), cdb::db_root(rt), rt->path};
    
    while(!queue.empty()){
        DQ1     dq;
        queue >> dq;
        
        dir::for_all_children(dq.path, dir::HIDDEN, [&](const std::filesystem::path& p){
            auto n  = p.filename().string();
            auto ln = to_lower(n);
            if(ignore(dq.folder, ln))
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
    
    CacheQuery  sn("SELECT id FROM " TBL_DOCUMENTS " WHERE name=? ORDER BY k");
    CacheQuery  sx("SELECT id FROM " TBL_DOCUMENTS " WHERE suffix=? ORDER BY k");
    CacheQuery  sf("SELECT id FROM " TBL_DOCUMENTS " WHERE folder=? ORDER BY k");
    CacheQuery  sfn("SELECT id FROM " TBL_DOCUMENTS " WHERE folder=? AND name=? ORDER BY k");
    CacheQuery  sfx("SELECT id FROM " TBL_DOCUMENTS " WHERE folder=? AND suffix=? ORDER BY k");
    
    for(const Stage3* h : handlers){
        SqlQuery::AutoFinish    af;
        auto& fs    = h->spec();
        switch(fs.type){
        case FileSpec::ByCacheDoc:
            af  = sn.autoFinish();
            sn.bind(1, fs.str);
            while(sn.step() == SQResult::Row){
                h -> invoke(Document{ sn.v_uint64(1)});
            }
            break;
        case FileSpec::ByCacheExt:
            af  = sx.autoFinish();
            sx.bind(1, fs.str);
            while(sx.step() == SQResult::Row)
                h -> invoke(Document{ sx.v_uint64(1)});
            break;
        case FileSpec::ByFolder:
            af  = sf.autoFinish();
            sf.bind(1, fs.u64);
            while(sx.step() == SQResult::Row)
                h -> invoke(Document{ sx.v_uint64(1)});
            break;
        case FileSpec::ByFolderDoc:
            af  = sfn.autoFinish();
            sfn.bind(1, fs.u64);
            sfn.bind(2, fs.str);
            while(sfn.step() == SQResult::Row)
                h -> invoke(Document{ sfn.v_uint64(1)});
            break;
        case FileSpec::ByFolderExt:
            af  = sfx.autoFinish();
            sfx.bind(1, fs.u64);
            sfx.bind(2, fs.str);
            while(sfx.step() == SQResult::Row)
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

std::vector<const Stage5*>  makeS5Handlers()
{
    std::vector<const Stage5*>   handlers    = Stage5::all();
    std::stable_sort(handlers.begin(), handlers.end(), [](const Stage5*a, const Stage5*b) -> bool {
        return a->order() < b->order();
    });
    return handlers;
}

void    stage5_runtime()
{
    static std::vector<const Stage5*>       handlers = makeS5Handlers();
    std::vector<Stage5::OneTime>            onces;
    Stage5::swap_one_times(onces);
    for(const Stage5* h : handlers)
        h->invoke();
    for(auto&& ot : onces)
        ot();
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

EnumMap<Change,Vector<const FileNotifier*>> change_map()
{
    EnumMap<Change,Vector<const FileNotifier*>> ret = FileNotifier::change_map();
    for(Change c : Change::all_values()){
        ret[c].stable_sort([](const FileNotifier*a, const FileNotifier*b) -> bool {
            return a->order() < b->order();
        });
    }
    return ret;
}


void    dispatch(Change change, Fragment frag, Folder fo, const std::filesystem::path& p)
{
    static EnumMap<Change,Vector<const FileNotifier*>> changeMap = change_map();

    std::string_view    name    = filename(p.native());
    std::string_view    ext     = file_extension(name);

    for(const FileNotifier* n : changeMap[change]){
        auto& fs = n->spec();
        switch(fs.type){
        case FileSpec::Never:
            continue;
        case FileSpec::Always:
            break;
        case FileSpec::ByFile:
            #ifdef WIN32
            if(!is_similar(p.string(), fs.str))
                continue;
            #else
            if(p.string() != fs.str)
                continue;
            #endif
            break;
        case FileSpec::ByExtension:
            if(!is_similar(ext, fs.str))
                continue;
            break;
        case FileSpec::ByFolder:
            if(fo.id != fs.u64)
                continue;
            break;
        case FileSpec::ByFolderDoc:
            if(fo.id != fs.u64)
                continue;
            if(!is_similar(name, fs.str))
                continue;
            break;
        case FileSpec::ByFolderExt:
            if(fo.id != fs.u64)
                continue;
            if(!is_similar(ext, fs.str))
                continue;
            break;
        case FileSpec::ByCacheDoc:
            if(!is_similar(name, fs.str))
                continue;
            break;
        case FileSpec::ByCacheExt:
            if(!is_similar(ext, fs.str))
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

        //  Stage 6 -- scan/sweep for changes
void    stage6_sweep()
{
    Deque<DQ1>      queue;
    
    for(const RootDir* rt : wksp::root_dirs())
        queue << DQ1{cdb::top_folder(), cdb::db_root(rt), rt->path};

    while(!queue.empty()){
        DQ1     dq;
        queue >> dq;
        
        // so we can process removes
        auto frags  = cdb::child_fragments_set(dq.directory);
        auto dirs   = cdb::child_directories_set(dq.directory);
        
        dir::for_all_children(dq.path, dir::HIDDEN, [&](const std::filesystem::path& p){
            auto n  = p.filename().string();
            auto ln = to_lower(n);
            if(ignore(dq.folder, ln))
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

void    pre_watch()
{
    for(const FileWatch* f : FileWatch::all()){
        if(!f) [[unlikely]]
            continue;
        Watch   w;
        w.watcher   = f;
        w.szt       = file_size_and_timestamp(f->file());
        gWatch.push_back(w);
    }
}

void    stage6_watch()
{
    for(Watch& w : gWatch){
        if(!w.watcher) [[unlikely]]
            continue;
            
        SizeTimestamp   szt = file_size_and_timestamp(w.watcher->file());
        if(szt != w.szt){
            w.watcher->handle();
            w.szt   = szt;
        }
    }
}


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
        
        auto& db    = wksp::db();
        pre_watch();

        while(gQuit == Quit::No){
            std::this_thread::sleep_for(10ms);
            stage5_runtime();
            stage6_sweep();
            stage6_watch();
            db.checkpoint();
        }
    });
    
    {
        std::unique_lock lk(mutex);
        cv.wait(lk, [&]{ return ready; });
    }
}

