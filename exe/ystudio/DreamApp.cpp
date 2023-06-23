////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DreamApp.hpp"

#include <QWebEngineSettings>

#include <mithrilApp/web/WebBrowser.hpp>

#include <basic/Url.hpp>
#include <gluon/core/Logging.hpp>
#include <io/Curl.hpp>
#include <io/PluginLoader.hpp>
#include <mithril/wksp/Workspace.hpp>
#include <sql/SqlLite.hpp>
#include <io/PluginLoader.hpp>
#include <nlohmann/json.hpp>

using namespace yq;
using namespace yq::gluon;
using namespace yq::mithril;


DreamApp*    DreamApp::s_app    = nullptr;

DreamApp::DreamApp(int& argc, char**argv) : yq::gluon::Application(argc, argv)
{
    if(!s_app)
        s_app   = this;
        
    //QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, false);
    //QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, false);
    //QWebEngineProfile::defaultProfile()->setUseForGlobalCertificateVerification();
}

DreamApp::~DreamApp()
{
    if(s_app == this)
        s_app = nullptr;
}

bool    DreamApp::configure()
{
    QStringList args    = arguments();
    if(args.size() < 2){
        yCritical() << "Insufficient arguments, expecting one... the port number";
        return false;
    }
    
    uint16_t    pp = args[1].toUShort();
    if(((pp < 1024) && (pp != 80) && (pp != 443)) || (pp > 49151)){
        yCritical() << "Invalid port number";
        return false;
    }
    
    Url     u;
    u.scheme    = "http";
    u.host      = "localhost";
    u.path      = "/api/wksp/quill";
    u.port      = (uint16_t) pp;
    
    Curl        contact;
    contact.set_url(u);
    
    HttpStatus hs   = contact.exec();
    if(!isSuccessful(hs)){
        yCritical() << "Unable to query workspace: " << hs.value() << " (" << statusMessage(hs) << ")";
        return false;
    }
   
    nlohmann::json  j   = contact.rx_json();
    std::string where   = j["quill"].get<std::string>();
    if(where.empty()){
        yCritical() << "No meaningful workspace discovered!";
        return false;
    }
    
    wksp::Config        cfg;
    cfg.spec        = where;
    cfg.app         = wksp::App::EDITOR;
    cfg.db_flags    = []() -> int { return SqlLite::ReadOnly; };
    if(!wksp::initialize(cfg))
        return false;
        
    m_home.setScheme("http");
    m_home.setHost("localhost");
    m_home.setPort(pp);
    m_home.setPath("/");
    
    WebBrowser::setHomeUrl(m_home);
    
    load_plugin_dir("plugin");
    //load_plugin_dir("plugin/ystudio");
    return true;
}


#include "moc_DreamApp.cpp"
