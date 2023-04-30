////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <basic/Logging.hpp>
#include <basic/TextUtils.hpp>
#include <basic/Url.hpp>
#include <sql/SqlLite.hpp>
#include <io/Curl.hpp>
#include <io/PluginLoader.hpp>
#include <meta/Meta.hpp>
#include <meta/ObjectInfoWriter.hpp>
#include <tachyon/Application.hpp>
#include <tachyon/Viewer.hpp>
#include <tachyon/ViewerCreateInfo.hpp>
#include <tachyon/ui/Widget.hpp>

#include <mithril/wksp/Workspace.hpp>

#include <imgui.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <cstdlib>
#include <fstream>
#include <sstream>

using namespace yq;
using namespace yq::mithril;
using namespace yq::tachyon;
using nlohmann::json;

nlohmann::json          gConfig;
std::filesystem::path   gConfigDir;
std::filesystem::path   gConfigFile;

/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MainWindow : public Widget {
    YQ_OBJECT_DECLARE(MainWindow, Widget)
public:

    MainWindow()
    {
    }
    
    ~MainWindow()
    {
    }
    
    void   imgui_(ViContext&u) override
    {
        using namespace ImGui;
        
        if(BeginMainMenuBar()){
            
        
            EndMainMenuBar();
        }

        ShowDemoWindow();
        
        Widget::imgui_(u);
    }
    
//    bool    edit(const std::filesystem::path& pth);

    static std::string  make_title(unsigned n=0)
    {
        std::ostringstream  oss;
        oss << "Your Quill: " << wksp::name() << " [" << wksp::author() << "]";
        if(n)
            oss << ": " << n;
        return oss.str();
    }


    std::string     m_status    = "Detangling Photons";
};

YQ_OBJECT_IMPLEMENT(MainWindow)

/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
    return true;
}

//  MOTTO:  Detangling Photons


int main(int argc, char* argv[])
{
    auto aci = std::make_shared<AppCreateInfo>();
    aci->app_name        = "Your Editor";
    aci->view.title      = "Your Quill: ";
    aci->view.clear      = { 0.0, 0.0, 0.0, 1. };
    aci->view.imgui      = true;
    
    Application app(argc, argv, aci);
    if(!configure(argc, argv))
        return -1;
    app.finalize();
        
    aci->view.title     = MainWindow::make_title();
    app.add_viewer(new MainWindow);
    app.run();
    return 0;
}
