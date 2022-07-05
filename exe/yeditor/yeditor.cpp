////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <basic/Logging.hpp>
#include <basic/PluginLoader.hpp>
#include <basic/meta/Meta.hpp>
#include <basic/meta/ObjectInfoWriter.hpp>
#include <engine/Application.hpp>
#include <engine/vulqan/VqUtils.hpp>
#include <engine/ImWindow.hpp>
#include <imgui.h>
#include <iostream>

using namespace yq;
using namespace yq::engine;

class EditorWindow : public ImWindow {
    YQ_OBJECT_DECLARE(EditorWindow, ImWindow)
public:
    EditorWindow(const ViewerCreateInfo & wci=ViewerCreateInfo ()) : ImWindow(wci)
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
    Ref<EditorWindow>   window  = new EditorWindow(wi);
    
    app.run_window(window.ptr(), 0.1);
    return 0;
}
