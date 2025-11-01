////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "config.hpp"
#include "yquill.hpp"

#include <yq/container/vector_utils.hpp>
#include <yq/core/BasicApp.hpp>
#include <yq/core/DelayInit.hpp>
#include <yq/core/Logging.hpp>

#include <yq/core/ThreadId.hpp>
#include <yq/file/DirUtils.hpp>
#include <yq/file/FileUtils.hpp>
#include <yq/process/PluginLoader.hpp>
#include <yq/sql/SqlLite.hpp>
#include <yq/sql/SqlQuery.hpp>
#include <yq/sql/SqlUtils.hpp>
#include <yq/text/match.hpp>
#include <yq/typedef/string.hpp>

#include <yq/meta/Meta.hpp>
#include <yq/mithril/wksp/Workspace.hpp>

#include <iostream>


using namespace yq;
using namespace yq::mithril;

volatile Quit   gQuit = Quit::No;

ConfigParams          configData;
const ConfigParams&   config = configData;

ConfigParams::ConfigParams() = default;
ConfigParams::~ConfigParams() = default;

void    sigQuit(int)
{
    gQuit   = Quit::Stop;
}

void    sigRestart(int)
{
    gQuit   = Quit::Restart;
}

void        run_scanner(Vector<std::thread>&);
void        run_server(Vector<std::thread>&);


int     db_flags()
{
    switch(thread::id()){
    case 0:
        return SqlLite::ReadWrite|SqlLite::Create;
    case 1:
    default:
        return SqlLite::ReadWrite;
//        return SqlLite::ReadOnly;
    }
}


bool    initialize(const char* wfile)
{
    //  EVENTUALLY.... better arguments, but for now.... 
    log_to_std_error(
    #ifndef NDEBUG
        LogPriority::Debug
    #endif
    );
    
    yInfo() << "Built with C++ " << __cplusplus;
    
    
    Meta::init();
    
    wksp::Config    wcfg;
    wcfg.spec       = wfile;
    wcfg.db_flags   = db_flags;

    if(!wksp::initialize(wcfg)){
        yCritical() << "Unable to initialize database!";
        return false;
    }
    
    if(!configData.port)
        configData.port = wksp::port();

    std::filesystem::path   plugin_dir  = BasicApp::app_dir() / "plugin" / "yserver";

    size_t n = load_plugin_dir(plugin_dir);
    DelayInit::init_all();
    auto& templates = wksp::templates();
    DelayInit::init_all();
    for_all_children(plugin_dir, dir::NO_FILES, [&](const std::filesystem::path& subdir){
        std::string s   = subdir.filename().string();
        if(templates.contains(s)){
            n += load_plugin_dir(subdir);
            DelayInit::init_all();
        }
    });
    for(auto& fp : wksp::resolve_all(".plugin"sv)){
        n += load_plugin_dir(fp);
        DelayInit::init_all();
    }
    yInfo() << "Loaded " << n << " Plugins.";
    Meta::init();
    
    const std::filesystem::path&   dbfile  = wksp::cache();
    
    if(std::filesystem::exists(dbfile)){
        std::error_code ec;
        if(!std::filesystem::remove(dbfile, ec)){
            yCritical() << "Unable to remove the old database at " << dbfile;
            return false;
        }
        
        auto shm    = dbfile;
        shm += "-shm";
        std::filesystem::remove(shm, ec);
        
        auto wal    = dbfile;
        wal += "-wal";
        std::filesystem::remove(wal, ec);
    }
    
    SqlLite& db = wksp::db();
    if(!db.is_open()){
        yCritical() << "Unable to create the database at " << dbfile;
        return false;
    }

    SqlQuery::exec(db, "PRAGMA journal_mode=WAL");
    
    path_vector_t   dbdirs  = wksp::shared_all("db"sv);
    dbdirs += wksp::resolve_all(".db"sv);
    
    dir::for_all_children(dbdirs, dir::NO_DIRS, [&](const std::filesystem::path& fname){
        if(!is_similar(fname.extension().c_str(), ".sql"))
            return ;
        db_run_script_file(fname, db);
    });
    
    Meta::init();

    wksp::set_db_init();
    
    n = 0;
    for(auto& fs : wksp::shared_dirs())
        yInfo() << "share directory " << (++n) << ": "  << fs;

    yInfo() << "Stage 1: File scan.";
    stage1_scan();    
    yInfo() << "Stage 2: Sweep.";
    stage2_unimport();
    yInfo() << "Stage 3: Digesting documents.";
    stage3_documents();
    yInfo() << "Stage 4: Finalization.";
    stage4_finalize();
    yInfo() << "Stage 5: Flushing write-ahead-log.";
    Meta::freeze();
    db.checkpoint();
    return true;
}



int execMain(int, char* argv[])
{
    if(!initialize(argv[1]))
        return -1;
    
    
    Vector<std::thread>     threads;
    
    run_scanner(threads);
    run_server(threads);
    
    //  eventually watch the IPC channel
    for(auto& t : threads)
        t.join();
    return 0;
}

void    print_help(const char* exe)
{
    std::cout << "Usage: " << exe << " (workspace)\n";
}

int main(int argc, char* argv[])
{
    if(argc <= 1){
        print_help(argv[0]);
        return 0;
    }

    BasicApp     app(argc, argv);
    int r = execMain(argc, argv);
    if((r == 0) && (gQuit == Quit::Restart)){
        yNotice() << "Restart requested.";
        std::vector<char*>   args(argv, argv+argc);
        args.push_back((char*) nullptr);
        execv(argv[0], args.data());
    }
    return r;
}

