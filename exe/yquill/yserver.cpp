#include "yquill.hpp"
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include <yq/http/HttpContext.hpp>
#include <yq/http/HttpDataStream.hpp>
#include <yq/http/HttpRequest.hpp>
#include <yq/http/HttpResponse.hpp>
#include <yq/http/HttpServer.hpp>
#include <yq/log/Logging.hpp>
#include <yq/text/Utils.hpp>
#include <yq/web/WebPage.hpp>
#include <yq/wksp/Workspace.hpp>

#include <fstream>
#include <syncstream>

using namespace yq;


namespace {
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
    
    void        do_server()
    {
        thread::id();
        yInfo() << "Launching Quill Server for '" << wksp::name() << "' on Port " << wksp::port() << ".";

        asio::error_code        ec;
        asio::io_context        context;
        HttpContext         env(context);
        env.handler     = http_process;
        env.version     = http10();
        env.port        = wksp::port();
        HttpServer  server(env);
        server.start();
        context.run();
        
        yInfo() << "Exiting the server.";
        
        if(gQuit == Quit::No)
            gQuit       = Quit::Stop;
    }
}


void        run_server(yq::Vector<std::thread>&  threads)
{
    threads << std::thread(do_server);
}

