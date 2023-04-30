////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "WorkspaceSelector.ipp"
#include "MainWindow.ipp"
#include "Config.ipp"
#include "History.ipp"

#include <basic/Logging.hpp>
#include <basic/TextUtils.hpp>
#include <sql/SqlLite.hpp>
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

using namespace yq;
using namespace yq::mithril;
using namespace yq::tachyon;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //!  Attempt the startup
    bool    init_workspace(const std::filesystem::path& pth)
    {
        wksp::Config        cfg;
        cfg.spec        = pth;
        cfg.app         = wksp::App::EDITOR;
        cfg.db_flags    = []() -> int { return SqlLite::ReadOnly; };
        if(!wksp::initialize(cfg))
            return false;
        
        Application::app()->finalize();
        return true;
    }

    //! Initializes from a port
    bool    init_workspace(uint16_t)
    {
        return false;
    }


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//  MOTTO:  Detangling Photons

int main(int argc, char* argv[])
{
    AppCreateInfo       aci;
    aci.app_name        = "Your Editor";
    aci.view.title      = "Your Editor!";
    aci.view.clear      = { 0.0, 0.0, 0.0, 1. };
    aci.view.imgui      = true;
    Application app(argc, argv, aci);
    load_plugin_dir("plugin");
    
    Config          cfg;
    MainWindow*     mw = nullptr;

    for(int i=1;i<argc;++i){
        if(is_similar(argv[i], "-p") && (++i < argc)){
            auto p  = to_uint16(argv[i]);
            if(!p){
                yCritical() << "Bad argument to port option!";
                return -1;
            }

            if(!init_workspace(*p)){
                yCritical()   << "Unable to initialize the workspace at port " << *p;
                return -1;
            }
        }
        
        if(!mw){
            if(!init_workspace(argv[i])){
                yCritical()   << "Unable to initialize the workspace at " << argv[i];
                return -1;
            }
            
            mw  =   new MainWindow(cfg);
            app.add_viewer(mw);
        }
        
        mw -> edit(argv[i]);
    }
    
    if(!mw)
        app.add_viewer(new WorkspaceSelector(cfg));
        
    app.run();
    return 0;
}
