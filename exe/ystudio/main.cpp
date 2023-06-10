////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DreamMW.hpp"
#include "DreamApp.hpp"

#include <basic/Logging.hpp>
#include <basic/Url.hpp>
#include <gluon/app/Application.hpp>
#include <mithril/wksp/Workspace.hpp>
#include <io/Curl.hpp>
#include <io/PluginLoader.hpp>
#include <sql/SqlLite.hpp>

#include <fstream>
#include <nlohmann/json.hpp>

using namespace yq;
using namespace yq::mithril;
using namespace yq::gluon;
using nlohmann::json;

nlohmann::json          gConfig;
std::filesystem::path   gConfigDir;
std::filesystem::path   gConfigFile;

bool    configure(int argc, char* argv[])
{
    if(argc < 2){
        yCritical() << "Insufficient arguments, expecting one... the port number";
        return false;
    }
    
    unsigned long   pp  = strtoul(argv[1], nullptr, 10);
    if(((pp < 1024) && (pp != 80) && (pp != 443)) || (pp > 49151)){
        yCritical() << "Invalid port number";
        return false;
    }
    
    auto    hdir    = BasicApp::user_home();
    if(!hdir.empty()){
        gConfigDir  = hdir / ".yquill";
        gConfigFile = gConfigDir / "config";
        
        if(std::filesystem::exists(gConfigFile)){
            std::ifstream    fin(gConfigFile);
            gConfig     = nlohmann::json::parse(fin);
        }
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
    
    load_plugin_dir("plugin");
    load_plugin_dir("plugin/ystudio");
    return true;
}

//  MOTTO:  Detangling Photons


int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName("YourQuill");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    DreamApp    app(argc, argv);
    if(!configure(argc, argv))
        return -1;
    app.finalize();
    (new DreamMW) -> show();
    
    return app.exec();
}
