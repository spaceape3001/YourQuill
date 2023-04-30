////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

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
#include <tachyon/widget/Stacked.hpp>

#include <mithril/wksp/Workspace.hpp>

#include <imgui.h>
#include <iostream>


using namespace yq;
using namespace yq::mithril;
using namespace yq::tachyon;

struct Config {
    std::vector<std::filesystem::path>      history;
    
    Config()
    {
    }
    
    ~Config()
    {
    }
    
    void    load()
    {
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MainWindow : public Widget {
    YQ_OBJECT_DECLARE(MainWindow, Widget)
public:

    Config&                         m_config;

    MainWindow(Config& cfg) : m_config(cfg)
    {
    }
    
    ~MainWindow()
    {
    }
    
    void   imgui_(tachyon::ViContext&u) override 
    {
        if(ImGui::BeginMainMenuBar()){
            
        
            ImGui::EndMainMenuBar();
        }

        ImGui::ShowDemoWindow();
    }


    bool    edit(const std::filesystem::path& pth)
    {
        return false;
    }
};

YQ_OBJECT_IMPLEMENT(MainWindow)

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


class WorkspaceSelector : public Widget {
    YQ_OBJECT_DECLARE(WorkspaceSelector, Widget)
public:

    Config& m_config;

    WorkspaceSelector(Config& cfg) : m_config(cfg) 
    {
    }
    
    ~WorkspaceSelector()
    {
    }
    
    
    void    toMain()
    {
        viewer() -> set_widget(new MainWindow(m_config));
    }
    
    void    attempt(const std::filesystem::path& pth)
    {
    }
    
    
    void   imgui_(ViContext&u) override 
    {
        ImGui::ShowMetricsWindow();
        ImGui::Begin("Workspace Selection", nullptr, ImGuiWindowFlags_NoCollapse);
        
            ImGui::Text("Workspace needs to be selected");
            
            //End();
        //}
        
        if(ImGui::Button("Okay")){
            toMain();
        }
        
        ImGui::End();
    }
    
    void        vulkan_(ViContext& ctx) override
    {
        //  a simple background will go here... 
    }
};
YQ_OBJECT_IMPLEMENT(WorkspaceSelector)


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
    cfg.load();
    
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
