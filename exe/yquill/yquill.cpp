#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include <yq/wksp/Workspace.hpp>
#include <yq/app/CmdArgs.hpp>
#include <yq/app/Plugins.hpp>
#include <yq/log/Logging.hpp>
#include <yq/meta/Meta.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/http/HttpContext.hpp>
#include <yq/http/HttpDataStream.hpp>
#include <yq/http/HttpRequest.hpp>
#include <yq/http/HttpResponse.hpp>
#include <yq/http/HttpServer.hpp>
#include <yq/text/Utils.hpp>
#include <yq/web/Web.hpp>

#include <iostream>
#include <fstream>
#include <memory>

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


std::pair<const Web*, std::string_view>     find_page(HttpOp m, std::string_view path)
{
    const Web*  p   = Web::page(m, path);
    if(p)
        return { p, std::string_view() };

    // yInfo() << path << " not found, trying directory";

    size_t  n   = path.find_last_of('/');
    if(n == std::string_view::npos)
        return {};
    if(!n)
        return {};
    p   = Web::directory(m, path.substr(0, n));
    if(p)
        return { p, path.substr(n+1) };

    // yInfo() << path.substr(0,n) << " not found, trying glob";
    
    for(; (n != std::string_view::npos) && n; n = path.find_last_of('/', n-1)){
        p       = Web::glob(m, path.substr(0, n));
        if(p)
            return { p, path.substr(n+1) };
    }
    
    return {};
}

void    http_process(const HttpRequest& rq, HttpResponse& rs)
{
yInfo() << "Request: " << rq.method() << ' ' << rq.url() << ' ' << rq.version();

    auto pg = find_page(rq.method(), rq.url().path);
    if(!pg.first)
        throw HttpStatus::NotFound;

    WebContext  ctx{rq, rs, pg.second};
    pg.first -> handle(ctx);
    if(rs.status() == HttpStatus())
        rs.status(HttpStatus::Success);
    return ;
}

int execMain(int argc, char* argv[])
{
    //  EVENTUALLY.... better arguments, but for now.... 
    log_to_std_error();
    Meta::init();

    if(!wksp::initialize(argv[1])){
        yError() << "Unable to initialize database!";
        return -1;
    }

    size_t n = load_plugin_dir("plugin");
    yInfo() << "Loaded " << n << " Plugins.";
    
    //  we'll eventually have a structure to the plugins.  For now, everything is loaded....
    //  (likely based off templates and/or keywords in the quill file)
    
    Meta::freeze();
    
    yInfo() << "Launching Quill Server for '" << wksp::name() << "' on Port " << wksp::port() << ".";
    
    HttpData::start_pool();
    
    //try {
        asio::error_code        ec;
        asio::io_context        context;
        HttpContext         env(context);
        env.handler     = http_process;
        env.version     = http10();
        env.port        = wksp::port();
        HttpServer              server(env);
        server.start();
        context.run();
    //}
    //catch(std::exception& e)
    //{
        
    //}

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

