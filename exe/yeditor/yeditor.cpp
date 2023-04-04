////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <basic/Logging.hpp>
#include <io/PluginLoader.hpp>
#include <meta/Meta.hpp>
#include <meta/ObjectInfoWriter.hpp>
#include <engine/Application.hpp>
#include <engine/Viewer.hpp>

#include <imgui.h>
#include <iostream>

using namespace yq;
using namespace yq::engine;

class EditorWindow : public Viewer {
    YQ_OBJECT_DECLARE(EditorWindow, Viewer)
public:
    EditorWindow(const ViewerCreateInfo & wci=ViewerCreateInfo ()) : Viewer(wci)
    {
    }
    
    ~EditorWindow()
    {
    }
    
    void   draw_imgui() override 
    {
        // temporary
        ImGui::ShowDemoWindow();
    }
};

YQ_OBJECT_IMPLEMENT(EditorWindow)


int main(int argc, char* argv[])
{
    AppCreateInfo        vi;
    vi.app_name     = "Your Editor";

    Application app(argc, argv, vi);
    load_plugin_dir("plugin");
    app.finalize();
    
    ViewerCreateInfo      wi;
    wi.title        = "Your Editor!";
    wi.clear        = { 0.0, 0.0, 0.0, 1. };
    wi.imgui        = true;
    Ref<EditorWindow>   window  = new EditorWindow(wi);
    
    app.run_window(window.ptr(), 0.1);
    return 0;
}
