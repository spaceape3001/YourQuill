////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <basic/Logging.hpp>
#include <io/PluginLoader.hpp>
#include <meta/Meta.hpp>
#include <meta/ObjectInfoWriter.hpp>
#include <tachyon/Application.hpp>
#include <tachyon/Viewer.hpp>
#include <tachyon/ViewerCreateInfo.hpp>
#include <tachyon/ui/Widget.hpp>

#include <imgui.h>
#include <iostream>

using namespace yq;
using namespace yq::tachyon;

class EditorWindow : public Widget {
    YQ_OBJECT_DECLARE(EditorWindow, Widget)
public:
    EditorWindow()
    {
    }
    
    ~EditorWindow()
    {
    }
    
    void   imgui_(tachyon::ViContext&u) override 
    {
        // temporary
        ImGui::ShowDemoWindow();
    }
};

YQ_OBJECT_IMPLEMENT(EditorWindow)

//  MOTTO:  Detangling Photons

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
    Ref<Viewer>   window  = new Viewer(wi, new EditorWindow);
    app.run(window.ptr(), {0.1});
    return 0;
}
