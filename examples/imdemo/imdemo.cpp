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
#include <engine/vulqan/VqApp.hpp>
#include <engine/vulqan/VqUtils.hpp>
#include <engine/vulqan/VqWindow.hpp>
#include <iostream>

using namespace yq;

int main(int argc, char* argv[])
{
    AppInfo        vi;
    vi.app_name     = "im_demo";
    vi.engine_name  = "YQ";

    VqApp app(argc, argv, vi);
    load_plugin_dir("plugin");
    app.finalize();
    
    VqWindow::Info      wi;
    wi.title        = "ImGUI Demo!";
    Ref<VqWindow>   window  = new VqWindow(wi);
    
    for(auto& si : vqGetPhysicalDeviceSurfaceFormatsKHR(window->physical(), window->surface())){
        yInfo() << "Available format: " << to_string(si.format) << " (" << to_string(si.colorSpace) << ")";
    }
    
    
    while(!window->should_close())
        VqWindow::poll_events();
    
    std::cout << "Hello World!\n";
    return 0;
}
