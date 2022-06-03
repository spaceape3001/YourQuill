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
#include <engine/vulqan/VqApp.hpp>
#include <engine/vulqan/VqUtils.hpp>
#include <engine/ui/ImWindow.hpp>
#include <imgui.h>
#include <iostream>

using namespace yq;

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
    vi.engine_name  = "YQ";

    VqApp app(argc, argv, vi);
    load_plugin_dir("plugin");
    app.finalize();
    
    WindowCreateInfo      wi;
    wi.title        = "ImGUI Demo!";
    wi.clear        = { 0.0, 0.2, 0.5, 1. };
    Ref<DemoWindow>   window  = new DemoWindow(wi);
    
    app.run(window.ptr());
    return 0;
}
