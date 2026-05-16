////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DreamApp.hpp"
#include "DreamMW.hpp"

#include <yq/core/Logging.hpp>
#include <yq/gluon/core/Utilities.hpp>
#include <yq/gluon/core/uurl.hpp>
#include <yq/meta/Meta.hpp>
#include <yq/mithril/wksp/Workspace.hpp>
#include <yq/net/Curl.hpp>
#include <yq/net/json.hpp>
#include <yq/process/PluginLoader.hpp>
#include <yq/sql/SqlLite.hpp>
#include <yq/text/parse.hpp>
#include <yqResources.hpp>

#include <QFont>
#include <QWebEngineSettings>

#include "WebBrowser.hpp"

#ifdef YIDE_VULKAN
#include <yq/assetvk/io/FileIOManager.hpp>
#include <yq/resource/Resource.hpp>
#include <yq/tachyon/api/Tachyon.hxx>
#include <yq/tedit/TEManager.hpp>
#include <yq/tedit/app/TEApp.hpp>
#include <yq/vkqt/app/YApp.hpp>
#endif

using namespace yq;
using namespace yq::gluon;
using namespace yq::mithril;
using namespace yq::tachyon;

//  MOTTO:  Detangling Photons

namespace G {
    Url         home_url;
    uint16_t    port;
    
#ifdef YIDE_VULKAN
    TEManager*  te_manager  = nullptr;
    TypedID     file_io;
#endif
};

const Url&                  gHomeUrl    = G::home_url;
const uint16_t&             gPort       = G::port;

#ifdef YIDE_VULKAN
TEManager* const&           gTEManager  = G::te_manager;
const TypedID&              gFileIO     = G::file_io;
#endif

bool    configureWorkspace(int argc, char* argv[])
{
    if(argc<2){
        yCritical() << "Insufficient arguments, expecting at least one (the port number)\n";
        return false;
    }
    
    uint16_x        px  = to_uint16(argv[1]);
    if(!px){
        yCritical() << "Bad port number";
        return false;
    }
    
    G::port           = *px;
    
    if(((gPort < 1024) && (gPort != 80) && (gPort != 443)) || (gPort > 49151)){
        yCritical() << "Invalid port number";
        return false;
    }
    
    Url     u{
        .scheme    = "http",
        .host      = "localhost",
        .path      = "/api/wksp/quill",
        .port      = gPort
    };
    
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
        
    G::home_url = Url{
        .scheme = "http",
        .host   = "localhost",
        .path   = "/",
        .port   = gPort
    };
    

    WebBrowser::setHomeUrl(qUrl(gHomeUrl));

    
    
    //load_plugin_dir("plugin/ystudio");
    return true;
    
}


int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName("YourQuill");
    DreamAppConfig  cfg;
    
    #ifdef YIDE_VULKAN
    cfg.thread.auxillary    = true;
    cfg.thread.edit         = true;
    cfg.thread.game         = true;
    cfg.thread.io           = true;
    cfg.thread.network      = true;
    cfg.thread.sim          = true;
    cfg.thread.task         = true;
    cfg.thread.viewer       = PER;
    cfg.view.title          = "Your Quill";
    cfg.view.size           = { 1920, 1080 };
    cfg.view.clear          = { 0.0f, 0.0f, 0.0f, 1.f };
    cfg.view.imgui          = true;
    cfg.view.resizable      = true;
    cfg.view.depth_buffer   = ENABLE;
    cfg.vulkan.graphics     = 16U;      // why not????
    #endif
    
    DreamApp    app(argc, argv, cfg);
    yq::Meta::init();

    log_to_std_error(); //  temporary

    if(!configureWorkspace(argc, argv))
        return -1;

    static const char* zPluginDirs[] = {
        "plugin",
        "plugin/yide",
        "plugin/xg",
        
        #ifdef YQ_YIDE_VULKAN
        "plugin/yidevk",
        "plugin/xgvk",
        "plugin/luavk",
        #endif
    };

    yq::Meta::init();

    #ifdef YIDE_VULKAN
    //app.vulqan_libraries(LOAD);
    //yq::Meta::init();
    #endif
    
    for(auto z : zPluginDirs){
        load_plugin_dir(z);
        yq::Meta::init();
    }


    load_plugin_dir("plugin");
    app.start();
    yq::resources::load_standard();
    yq::Meta::freeze();
    
    {
        DreamMW *   w   = new DreamMW;
        w -> newBrowser();
        w -> show();
    }
    
    if(!app.qtApp())
        return -1;
    
    #ifdef YIDE_VULKAN
    #endif

    return app.qtApp()->exec();
}

