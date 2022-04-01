////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include <yq/wksp/Workspace.hpp>

#include <yq/app/CmdArgs.hpp>
#include <yq/app/Plugins.hpp>
#include <yq/file/FileUtils.hpp>
#include <yq/log/Logging.hpp>
#include <yq/meta/Meta.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/http/HttpContext.hpp>
#include <yq/http/HttpDataStream.hpp>
#include <yq/http/HttpRequest.hpp>
#include <yq/http/HttpResponse.hpp>
#include <yq/http/HttpServer.hpp>
#include <yq/sql/SqlLite.hpp>
#include <yq/sql/SqlUtils.hpp>
#include <yq/text/Utils.hpp>
#include <yq/web/WebPage.hpp>

#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <syncstream>

//#include "HttpConnection.hpp"
//#include "HttpConnection.ipp"


using namespace yq;
using asio::ip::tcp;

enum class Quit {
    No      = 0,
    Stop,
    Restart
};

volatile Quit   gQuit = Quit::No;


void    sigQuit(int)
{
    gQuit   = Quit::Stop;
}

void    sigRestart(int)
{
    gQuit   = Quit::Restart;
}

static const char  szHelloWorld[] = "<html><body><h1>HELLO WORLD!</h1></body></html>";

static constexpr const size_t       kBufferSize     = 8192;


std::pair<const WebPage*, std::string_view>     find_page(HttpOp m, std::string_view path)
{
    const WebPage*  p   = web::page(m, path);
    if(p)
        return { p, std::string_view() };

    // yInfo() << path << " not found, trying directory";

    size_t  n   = path.find_last_of('/');
    if(n == std::string_view::npos)
        return {};
    if(!n)
        return {};
    p   = web::directory(m, path.substr(0, n));
    if(p)
        return { p, path.substr(n+1) };

    // yInfo() << path.substr(0,n) << " not found, trying glob";
    
    for(; (n != std::string_view::npos) && n; n = path.find_last_of('/', n-1)){
        p       = web::glob(m, path.substr(0, n));
        if(p)
            return { p, path.substr(n+1) };
    }
    
    return {};
}

std::filesystem::path     access_log_file()
{
    std::string     file    = "access-";
    file += wksp::start_file();
    file += '-';
    file += to_string(thread::id());
    file += ".log";
    return wksp::log_dir() / file;
}

void    http_process(const HttpRequest& rq, HttpResponse& rs)
{
//yInfo() << "Request: " << rq.method() << ' ' << rq.url() << ' ' << rq.version();

    static thread_local std::ofstream access_log(access_log_file(), std::ios_base::out | std::ios_base::app);

    HttpStatus  status;
    
    time_t  atime;
    time(&atime);
    struct tm   ftime;
    char    timestamp[256];
    gmtime_r(&atime, &ftime);
    strftime(timestamp, sizeof(timestamp), "%F %T UTC", &ftime);
    
    do {
        auto pg = find_page(rq.method(), rq.url().path);
        if(!pg.first){
            status = HttpStatus::NotFound;
            break;
        }
        
        WebContext  ctx{rq, rs, pg.second};
        ctx.time_text   = std::string_view(timestamp);
        ctx.time        = atime;
        
        bool    isLocal = rq.remote_addr().is_loopback();
        if(isLocal)
            ctx.flags |= WebContext::LOCAL;

        if(pg.first -> local_only() && !isLocal){
            status = HttpStatus::Forbidden;
            break;
        }
        
        try {
            pg.first -> handle(ctx);
        }
        catch(HttpStatus s)
        {
            status  = s;
        }
        catch(HttpStatus::enum_t s)
        {
            status = s;
        }
    } while(false);

    if(status != HttpStatus()){
        rs.status(status);
    } else
        rs.status(HttpStatus::Success);
    
    // log it.
    access_log << rq.remote_addr().to_string() 
        << " - [" << timestamp  << "] \"" << rq.method().key() << " " << rq.url().path << " " << rq.version() 
        << "\" " << rs.status().value() << " " << rs.content_size() << "\n";
    access_log.flush();
    
    #ifndef NDEBUG
    yInfo() << rq.remote_addr().to_string() 
        << " - [" << timestamp  << "] \"" << rq.method() << " " << rq.url().path << " " << rq.version() 
        << "\" " << rs.status().value() << " " << rs.content_size();
    #endif

    return ;
}

int     db_flags()
{
    switch(thread::id()){
    case 0:
        return SqlLite::ReadWrite|SqlLite::Create;
    case 1:
        return SqlLite::ReadWrite;
    default:
        return SqlLite::ReadOnly;
    }
}

void    dir_monitor()
{
    using namespace std::chrono_literals;
    //uint64_t    cnt = 0;
    //  TODO....
    while(gQuit == Quit::No){
        std::this_thread::sleep_for(5s);
        //yInfo() << "Dir monitor thread, tick " << ++cnt;
    }
}

bool    initialize(const char* wfile)
{
    //  EVENTUALLY.... better arguments, but for now.... 
    log_to_std_error();
    Meta::init();
    
    wksp::Config    wcfg;
    wcfg.spec       = wfile;
    wcfg.db_flags   = db_flags;

    if(!wksp::initialize(wcfg)){
        yCritical() << "Unable to initialize database!";
        return false;
    }

    size_t n = load_plugin_dir("plugin");
    for(auto& fp : wksp::resolve_all(".plugin"sv))
        n += load_plugin_dir(fp);
    yInfo() << "Loaded " << n << " Plugins.";
    Meta::init();
    
    const std::filesystem::path&   dbfile  = wksp::cache();
    
    if(std::filesystem::exists(dbfile)){
        std::error_code ec;
        if(!std::filesystem::remove(dbfile)){
            yCritical() << "Unable to remove the old database at " << dbfile;
            return false;
        }
    }
    
    SqlLite& db = wksp::db();
    if(!db.is_open()){
        yCritical() << "Unable to create the database at " << dbfile;
        return false;
    }
    
    path_vector_t   dbdirs  = wksp::shared_all("db"sv);
    dbdirs += wksp::resolve_all(".db"sv);
    
    dir::for_all_children(dbdirs, dir::NO_DIRS, [&](const std::filesystem::path& fname){
        if(!is_similar(fname.extension().c_str(), ".sql"))
            return ;
        db_run_script_file(fname, db);
    });
    
    

    HttpData::start_pool();

    Meta::freeze();
    return true;
}



int execMain(int argc, char* argv[])
{
    if(!initialize(argv[1]))
        return -1;
    
    
    Vector<std::thread>     threads;
    
    std::condition_variable cv;
    std::mutex              mutex;
    bool                    ready = false;
    
    threads << std::thread([&](){
        thread::id();
        ready       = true;
        cv.notify_one();
        dir_monitor();
    });
    
    {
        std::unique_lock lk(mutex);
        cv.wait(lk, [&]{ return ready; });
    }
    
    threads << std::thread([&](){
        thread::id();

        yInfo() << "Launching Quill Server for '" << wksp::name() << "' on Port " << wksp::port() << ".";

        asio::error_code        ec;
        asio::io_context        context;
        HttpContext         env(context);
        env.handler     = http_process;
        env.version     = http10();
        env.port        = wksp::port();
        HttpServer              server(env);
        server.start();
        context.run();
        
        yInfo() << "Exiting the server.";
        
        if(gQuit == Quit::No)
            gQuit       = Quit::Stop;
    });

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

    CmdArgs     args(argc, argv);
    int r = execMain(argc, argv);
    if((r == 0) && (gQuit == Quit::Restart)){
        yNotice() << "Restart requested.";
        std::vector<char*>   args(argv, argv+argc);
        args.push_back((char*) nullptr);
        execv(argv[0], args.data());
    }
    return r;
}

