////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

/*
    Simple "demo" utilitiy
*/

#include <basic/Logging.hpp>
#include <basic/PluginLoader.hpp>
#include <basic/meta/Meta.hpp>
#include <basic/meta/ObjectInfoWriter.hpp>
#include <engine/Application.hpp>
#include <engine/vulqan/VqUtils.hpp>
#include <engine/ui/ImWindow.hpp>
#include <imgui.h>
#include <iostream>

using namespace yq;
using namespace yq::engine;

class DemoWindow : public ImWindow {
    YQ_OBJECT_DECLARE(DemoWindow, ImWindow)
public:
    DemoWindow(const WindowCreateInfo & wci=WindowCreateInfo ()) : ImWindow(wci)
    {
    }
    
    ~DemoWindow()
    {
    }
    
    void   draw_imgui() override 
    {
        ImGui::ShowDemoWindow();
    }
};

YQ_OBJECT_IMPLEMENT(DemoWindow)

int main(int argc, char* argv[])
{
    AppCreateInfo        vi;
    vi.app_name     = "im_demo";

    Application app(argc, argv, vi);
    load_plugin_dir("plugin");
    app.finalize();
    
    WindowCreateInfo      wi;
    wi.title        = "ImGUI Demo!";
    wi.clear        = { 0.0, 0.2, 0.5, 1. };
    wi.resizable    = true;
    Ref<DemoWindow>   window  = new DemoWindow(wi);
    
    app.run_window(window.ptr(), 0.1);
    return 0;
}
