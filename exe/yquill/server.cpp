////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <db/ShareDir.hpp>
#include <db/Wksp.hpp>

#include <srv/Page.hpp>
#include <srv/Scanner.hpp>
#include <srv/Session.hpp>
#include <srv/TLSGlobals.hpp>

#include <yq/CmdArgs.hpp>
#include <yq/FileUtils.hpp>
#include <yq/LogFile.hpp>
#include <yq/Logging.hpp>
#include <yq/Safety.hpp>
#include <yq/SqlUtils.hpp>

#include <httpserver/httpcookie.h>
#include <httpserver/httplistener.h>
#include <httpserver/httprequest.h>
#include <httpserver/httprequesthandler.h>
#include <httpserver/httpresponse.h>

#include <QCoreApplication>
#include <QDir>
#include <QSqlDatabase>
#include <QThreadPool>
#include <QUrl>

#include <fstream>

#include <signal.h>
#include <unistd.h>

//#include "yCommon.hpp"
//#include "yScanner.hpp"
//#include "yNetWriter.hpp"
//#include "yPage.hpp"

//#include "db/Cache.hpp"
//#include "util/Guarded.hpp"
//#include "util/Ref.hpp"
//#include "util/Utilities.hpp"

//#include <QDateTime>
//#include <QHostAddress>

//#include <filesystem>
//#include <sys/random.h>
//#include <tbb/spin_rw_mutex.h>

using namespace stefanfrings;

const char* const       kCookieComment         = "Cookies are tasty!";
const char* const       kCookieDomain          = "yquill.localhost";
const char* const       kCookieName            = "QuillSessionId";  
const unsigned int      kCookieExpire          = 300;
const unsigned int      kAutoLogoutInterval    = 900;

bool                                g_can_remote        = true;



//namespace {
    //using PageMap =  Map<String,const Page*,IgCase>;

    //struct Repo {
        //EnumMap<HttpOp,PageMap>         pages;
        //GetMap                          getters;
        //Guarded<QByteArray>             t_page;
        ////Map<String,QDir,IgCase>         dirs;
        //PageMap                         dispatchers;
      
        //Repo() : t_page(shared_bytes("std/page"))
        //{
            ////  Only hook the dynamic ones here.....
            //getters["body"] = []()->QByteArray {
                //return x_content;
            //};
            //getters["title"] = []()->QByteArray {
                //return x_title;
            //};
        //}
    //};
    
    //Repo&   repo() 
    //{ 
        //static Repo*    s_repo = new Repo;
        //return *s_repo;
    //}
//}


void    updater_init();


Ref<Session>      sessionFor(HttpRequest& req, HttpResponse& resp)
{
    QByteArray  ssId    = resp.getCookies().value(kCookieName).getValue();
    if(ssId.isEmpty())
        ssId        = req.getCookie(kCookieName);
    ssId            = QByteArray::fromBase64(ssId);
    Ref<Session>    ret = sessionFor(ssId);
    resp.setCookie(HttpCookie(
        kCookieName,
        ret -> net().toBase64(),
        kCookieExpire,
        "/",
        QByteArray(), QByteArray(), false, true 
    ));
    ret -> accessed = QDateTime::currentSecsSinceEpoch();
    return ret;
}



Page::Reply    do_dispatch()
{
    //static Repo& _repo = repo();
    Page::Reply   ret;

    try {
        do {
            if(!x_request)
                throw HttpStatus::InternalError;
            if(!x_session)
                throw HttpStatus::InternalError;
                
                //  First, check the path for anything...fishy
            if(x_path.isEmpty())
                throw HttpStatus::NotAcceptable;
            if(x_path[0] != '/')
                throw HttpStatus::NotAcceptable;
            if(x_path.indexOf("/.") >= 0)
                throw HttpStatus::NotAcceptable;
            if(x_path.indexOf("//") >= 0)
                throw HttpStatus::NotAcceptable;
            for(char ch : x_path){
                if(ch <= ' ')           // no spaces
                    throw HttpStatus::NotAcceptable;
                if(ch == '\\')          // no funny slashes
                    throw HttpStatus::NotAcceptable;
                if(!isgraph(ch))
                    throw HttpStatus::NotAcceptable;
            }
                
            x_page      = Page::find(x_op, x_path, false);
            if(!x_page && (x_op == hGet)){
                int i   = x_path.indexOf('/', 1);
                if(i>0){
                    x_pathinfo  = x_path.mid(i+1);
                    x_page      = Page::find(x_op, x_path.mid(1,i-1), true);
                }
            }
            
            if(x_page){
                if(x_page->local_only() && !x_is_local)
                    throw HttpStatus::Forbidden;
                    
                    //  Permission filters....
                switch(x_op){
                case hGet:
                    if(x_page->login_required() && !x_session->loggedIn)
                        throw HttpStatus::Unauthorized;
                    break;
                case hPost:
                    if(!x_page->post_anon() && !x_session->loggedIn && !x_is_local)
                        throw HttpStatus::Unauthorized;
                    break;
                default:
                    if(!x_session->loggedIn && !x_is_local)
                        throw HttpStatus::Unauthorized;
                    break;
                }
                
                ByteArrayStream stream(ret.content);

                ret.type    = x_page -> handle(stream);
                ret.status  = HttpStatus::Success;
                break;
            }
            
            
            throw HttpStatus::NotFound;
        } while(false);
    } 
    catch(HttpStatus x){
        ret.status      = x;
        ret.content     = statusMessage(x);
    }
    catch(HttpStatus::enum_t x){
        ret.status      = x;
        ret.content     = statusMessage(x);
    }
    catch(Redirect r){
        ret.status      = r.why;
        ret.content     = r.where;
    }
    catch(const QUrl& url){
        ret.status      = HttpStatus::TemporaryRedirect;
        ret.content     = url.toString().toUtf8();
    }
    catch(const std::exception& ex) {
        ret.status  = HttpStatus::InternalError;
        ret.content = statusMessage(ret.status);
    }
    return ret;
}



struct DBServer : public HttpRequestHandler {
    QString         m_accessLog;
    //QString         m_errorLog;
    
    DBServer()
    {
        QDir            logDir(QString::fromStdString(wksp::log_dir().string()));
        m_accessLog         = logDir.absoluteFilePath(QString("access-%1").arg(wksp::start().qString()) + "-%1.log");
        //m_errorLog          = logDir.absoluteFilePath(QString("error-%1").arg(wksp::start()) + "-%1.log");
    }

    void    service(HttpRequest& req, HttpResponse& resp) override
    {
        static thread_local LogFile* accessLog   = new LogFile(m_accessLog);
        
        try {
                // ensure old stuff gets cleared, even in case of an exception
            auto clear_tls  = safety([&](){ reset_tls(); });
            
            x_request           = &req;
            x_response          = &resp;
            x_op                = HttpOp(req.getMethod().constData());
            x_at                = QDateTime::currentDateTimeUtc();;
            x_time              = x_at.toString(Qt::ISODate);
            QHostAddress        peer = req.getPeerAddress();
            x_client            = peer.toString().toUtf8();
            x_is_local          = peer.isLoopback();
            
            if((!x_is_local) && !g_can_remote)
                throw HttpStatus::Forbidden;
            
            Ref<Session>  dbs = sessionFor(req,resp);
            x_session           = dbs.ptr();
            x_path              = req.getPath();
            x_path.replace('\\','/');
            x_can_edit          = x_is_local || x_session -> loggedIn;
            
            Page::Reply     rep = do_dispatch();
            
            if(isRedirect(rep.status)){
                resp.setStatus(rep.status);
                resp.redirect(rep.content);
            } else if(isError(rep.status)){
                resp.setStatus(rep.status.value(), rep.content);
            } else {
                resp.setStatus(rep.status.value());
                resp.setHeader("Content-Type", mimeType(rep.type));
                resp.write(std::move(rep.content), true);
            }
            
            accessLog -> line() << x_session->user << " -- " << x_client << " -- " << x_time << " -- " 
                << req.getMethod() << " -- \"" << x_path << "\" -- " << rep.status.value();

        } catch(HttpStatus::enum_t e)
        {
            resp.setStatus((int) e, statusMessage(e));
        }
    }
};


int main(int argc, char* argv[])
{
    if(argc < 2){
        std::cerr << "Usage: " << argv[0] << " (workspace)\n";
        return -1;
    }
    
    CmdArgs     cargs(argc, argv);
    
    log_to_std_error();
    QCoreApplication    app(argc, argv);
    if(!QThreadPool::globalInstance())
        yWarning() << "Global Thread Pool is NULL!";
    meta_init();
    meta_freeze();
    if(!wksp::initialize(argv[1], wksp::SEARCH|wksp::TEMPLATES_RO|wksp::INIT_LOG|wksp::BAIL_PID)){
        yError() << "Unable to initialize database!";
        return -1;
    }
    
    //  TODOs
    //  1. Guard on the dup PID (check, if readable, kill the other
    //  2. Move the database wipe/init here
    
    std::filesystem::path   pidfragment = wksp::db_pid();
    if(file_exists(pidfragment.c_str())){
        auto    i   = String(file_load_all(pidfragment.c_str())).to_uinteger();
        if(i.good){
            if(kill((pid_t) i.value, SIGTERM)){
                yWarning() << "Loitering PID fragment detected!";
            } else {
                yInfo() << "Terminated (gracefully) existing process.";
                usleep(50); // wait for the existing program to exit
            }
        }
    }
    {
        std::ofstream   p(pidfragment);
        p << getpid();
    }
    
    {
        QString  df  = QString::fromStdString(wksp::cache_db().string());
        if(QFile::exists(df))
            QFile::remove(df);
    }

        // Initialize the database
    for(QDir dbDir : shared_dirs("db")){
        for(QString s : dbDir.entryList(QStringList() << "*.sql", QDir::NoFilter, QDir::Name)){
            QString fs  = dbDir.absoluteFilePath(s);
            yInfo() << "Running db-init file " << fs;
            if(!db_run_script_file(fs, wksp::cache()))
                yWarning() << "Cache unable to initialize from file " << fs << "!";
        }
    }
    

    QSettings       *ini = new QSettings( QString::fromStdString(wksp::ini().string()), QSettings::IniFormat);
    ini->setValue("port", wksp::port());
    ini->setValue("minThreads", 10);
    ini->setValue("readTimeout", wksp::read_timeout());
    ini->setValue("cleanupInterval", 10000);
    ini->sync();

    Scanner*fs      = new Scanner;
    fs -> prime();
    updater_init();
    Page::freeze();
    
    fs -> start();
    DBServer  server;
    
    new HttpListener(ini, &server, &server);
    yInfo() << "Server started on " << wksp::port() << " for " << wksp::name();
    int r = app.exec();
    //fs -> quit();
    std::filesystem::remove(pidfragment);
    return r;
}
