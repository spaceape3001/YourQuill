////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "yCommon.hpp"
#include "yScanner.hpp"
#include "yNetWriter.hpp"
#include "yPage.hpp"

#include "db/Cache.hpp"
#include "db/ShareDir.hpp"
#include "util/FileUtils.hpp"
#include "util/LogFile.hpp"
#include "util/Logging.hpp"
#include "util/Guarded.hpp"
#include "util/Ref.hpp"
#include "util/SqlUtils.hpp"
#include "util/Utilities.hpp"

#include <httpserver/httpcookie.h>
#include <httpserver/httplistener.h>
#include <httpserver/httprequest.h>
#include <httpserver/httprequesthandler.h>
#include <httpserver/httpresponse.h>

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QHostAddress>
#include <QSqlDatabase>
#include <QThreadPool>
#include <QUrl>

#include <filesystem>
#include <fstream>
#include <signal.h>
#include <unistd.h>
#include <sys/random.h>
#include <tbb/spin_rw_mutex.h>

using namespace stefanfrings;

const char* const       kCookieComment         = "Cookies are tasty!";
const char* const       kCookieDomain          = "nakedpyro.com";
const char* const       kCookieName            = "QuillSessionId";  
const unsigned int      kCookieExpire          = 300;
const unsigned int      kAutoLogoutInterval    = 900;

thread_local HttpRequest*           x_request          = nullptr;
thread_local HttpResponse*          x_response         = nullptr;
thread_local QDateTime              x_at;
thread_local QString                x_time;
thread_local bool                   x_is_local         = false;
thread_local bool                   x_can_edit         = false;
thread_local QByteArray             x_client;
thread_local HttpOp                 x_op;
thread_local QByteArray             x_path;
thread_local Session*               x_session          = nullptr;
thread_local QByteArray             x_title;
thread_local QByteArray             x_content;
thread_local Vector<QByteArray>     x_scripts;

bool                                x_can_remote        = true;

thread_local Atom                   x_atom;
thread_local Class                  x_class;
thread_local Directory              x_directory;
thread_local Document               x_document;
thread_local Field                  x_field;
thread_local Folder                 x_folder;
thread_local Fragment               x_fragment;
thread_local Graph                  x_graph;
thread_local Image                  x_icon;
thread_local Leaf                   x_leaf;
thread_local const Page*            x_page              = nullptr;
thread_local const Root*            x_root              = nullptr;
thread_local Tag                    x_tag;
thread_local bool                   x_edit_req          = false;
thread_local QString                x_key;
thread_local unsigned int           x_columns           = 0;


namespace {
    using PageMap =  Map<String,const Page*,IgCase>;

    struct Repo {
        EnumMap<HttpOp,PageMap>         pages;
        GetMap                          getters;
        Guarded<QByteArray>             t_page;
        //Map<String,QDir,IgCase>         dirs;
        PageMap                         dispatchers;
      
        Repo() : t_page(shared_bytes("std/page"))
        {
            //  Only hook the dynamic ones here.....
            getters["body"] = []()->QByteArray {
                return x_content;
            };
            getters["title"] = []()->QByteArray {
                return x_title;
            };
        }
    };
    
    Repo&   repo() 
    { 
        static Repo*    s_repo = new Repo;
        return *s_repo;
    }
}

QByteArray      def_page()
{
    return repo().t_page;
}

void    reg_page(Page* p)
{
    if(p->is_dispatcher())
        repo().dispatchers[p->path()]           = p;
    else
        repo().pages[p->httpOp()][p->path()]    = p;
}


void    reg_getter(const char*z, FNGet fn)
{
    if(z && fn)
        repo().getters[z]   = fn;
}

void    reg_def_page(const QByteArray& bytes)
{
    repo().t_page   =    bytes;
}


void    updater_init();

const GetMap&   static_getters()
{
    return repo().getters;
}


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


QByteArray                   do_expand(const QByteArray&content, const GetMap&vars)
{
    static const Repo& r   = repo();

    QByteArray      ret;
    const char*     z   = content.constData();
    int             n = 0;
    int             i = 0;
    while( (i = content.indexOf("{{", n)) >= 0){
        ret.append(z+n, i-n);
        n = i + 2;
        i   = content.indexOf("}}", n);
        if(i < 0){
            n = i = -1;
            break;
        }
        
        QByteArray k    = QByteArray(z+n, i-n);
        FNGet   fn  = vars.get(k,nullptr);
        if(!fn)
            fn  = r.getters.get(k,nullptr);
        if(fn)
            ret.append(fn());
        n   = i + 2;
    }
    
    if(n >= 0)
        ret.append(z+n);
    return ret;
}


ContentType                  do_direct_content(QByteArray&dst, const QByteArray&data, ContentType ret, const QByteArray&title)
{
    switch(ret){
    case ContentType::html:
        x_title     = title;
        x_content   = data;
        dst         = do_expand(repo().t_page);
        return ContentType::html;
    case ContentType::markdown:
        {
            auto ct     = Markdown::exec(data);
            x_title     = ct.title;
            if(x_title.isEmpty())
                x_title = title;
            x_content   = ct.content;
            dst         = do_expand(repo().t_page);
            return ContentType::html;
        }
        return ContentType::html;
    case ContentType::text:
        x_title     = title;
        x_content   = "<PRE>" + data + "</PRE>";
        dst         = do_expand(repo().t_page);
        return ContentType::html;
    default:
        break;
    };
    dst     = data;
    return ret;
}


ContentType                 do_direct_file(QByteArray& dst, const QString&file, bool fExpand)
{
    QByteArray  data    = file_bytes(file);
    QFileInfo   fi(file);
    ContentType ret = mimeTypeForExt(fi.suffix());
    switch(ret){
    case ContentType::html:
        if(fExpand){
            x_title     = fi.fileName().toUtf8();
            x_content   = std::move(data);
            dst         = do_expand(repo().t_page);
            return ContentType::html;
        }
        break;
    case ContentType::markdown:
        if(fExpand){
            auto ct     = Markdown::exec(data);
            x_title     = ct.title;
            if(x_title.isEmpty())
                x_title = fi.fileName().toUtf8();
            x_content   = ct.content;
            dst         = do_expand(repo().t_page);
            return ContentType::html;
        }
        break;
    case ContentType::text:
        if(fExpand){
            x_content   = "<PRE>" + data + "</PRE>";
            x_title     = fi.fileName().toUtf8();
            dst         = do_expand(repo().t_page);
            return ContentType::html;
        }
        break;
    default:
        break;
    }
    dst     = std::move(data);
    return ret;
}

Page::Reply    do_dispatch()
{
    static Repo& _repo = repo();
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
                
            QByteArray  k;
            x_page      = _repo.pages[x_op].get(x_path, nullptr);
            if(!x_page && (x_op == hGet)){
                int i   = x_path.indexOf('/', 1);
                if(i>0){
                    k           = x_path.mid(i+1);
                    x_page      = _repo.dispatchers.get(x_path.mid(1,i-1), nullptr);
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

                ret.type    = x_page -> handle(ret.content, k);
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
        QDir            logDir(wksp::log_dir_path());
        m_accessLog         = logDir.absoluteFilePath(QString("access-%1").arg(wksp::start()) + "-%1.log");
        //m_errorLog          = logDir.absoluteFilePath(QString("error-%1").arg(wksp::start()) + "-%1.log");
    }

    void    service(HttpRequest& req, HttpResponse& resp) override
    {
        static thread_local LogFile* accessLog   = new LogFile(m_accessLog);
        
        try {
            x_request           = &req;
            x_response          = &resp;
            x_op                = HttpOp(req.getMethod().constData());
            x_at                = QDateTime::currentDateTimeUtc();;
            x_time              = x_at.toString(Qt::ISODate);
            QHostAddress        peer = req.getPeerAddress();
            x_client            = peer.toString().toUtf8();
            x_is_local          = peer.isLoopback();
            
            if((!x_is_local) && !x_can_remote)
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
                resp.write(rep.content, true);
            }
            
            accessLog -> line() << x_session->user << " -- " << x_client << " -- " << x_time << " -- " 
                << req.getMethod() << " -- \"" << x_path << "\" -- " << rep.status.value();

            // Clear out the old stuff....

            x_session   = nullptr;
            x_request   = nullptr;
            x_response  = nullptr;
            x_scripts.clear();
            x_page      = nullptr;
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
    
    log_to_std_error();
    QCoreApplication    app(argc, argv);
    if(!QThreadPool::globalInstance())
        yWarning() << "Global Thread Pool is NULL!";
    meta_init();
    meta_freeze();
    if(!wksp::initialize(argv[1], all_set<wksp::Option>())){
        yError() << "Unable to initialize database!";
        return -1;
    }
    
    //  TODOs
    //  1. Guard on the dup PID (check, if readable, kill the other
    //  2. Move the database wipe/init here
    
    std::filesystem::path   pidfragment = wksp::db_pid().toStdString();
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
        const QString&  df  = wksp::cache_db();
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
    

    QSettings       *ini = new QSettings( wksp::ini(), QSettings::IniFormat);
    ini->setValue("port", wksp::port());
    ini->setValue("minThreads", 10);
    ini->setValue("readTimeout", wksp::read_timeout());
    ini->setValue("cleanupInterval", 10000);
    ini->sync();

    Scanner*fs      = new Scanner;
    fs -> prime();
    updater_init();
    
    fs -> start();
    DBServer  server;
    
    new HttpListener(ini, &server, &server);
    yInfo() << "Server started on " << wksp::port() << " for " << wksp::name();
    int r = app.exec();
    //fs -> quit();
    std::filesystem::remove(pidfragment);
    return r;
}
