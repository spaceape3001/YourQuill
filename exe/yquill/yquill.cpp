#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include <db/wksp/Workspace.hpp>
#include <util/app/CmdArgs.hpp>
#include <util/log/Logging.hpp>
#include <util/meta/Meta.hpp>
#include <util/stream/Ops.hpp>
#include <io/http/HttpContext.hpp>
#include <io/http/HttpDataStream.hpp>
#include <io/http/HttpRequest.hpp>
#include <io/http/HttpResponse.hpp>
#include <io/http/HttpServer.hpp>

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


void    http_process(const HttpRequest& rq, HttpResponse& rs)
{
    HttpDataStream  out(rs.content(ContentType::html));
    out << "<html><head><title>HELLO WORLD!</title></head>" 
        << "<body><h1>HELLO WORLD!</h1></body></html>\n";

//    rs.status(HttpStatus::NotImplemented);
}

int execMain(int argc, char* argv[])
{
    //  EVENTUALLY.... better arguments, but for now.... 
    log_to_std_error();
    Meta::init();
    
    //  TODO .... load plugins
    
    Meta::freeze();
    if(!wksp::initialize(argv[1])){
        yError() << "Unable to initialize database!";
        return -1;
    }
    
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

