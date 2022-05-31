////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

/*
    Simple "demo" utilitiy
*/

#include <basic/BasicApp.hpp>
#include <basic/Logging.hpp>
#include <basic/PluginLoader.hpp>
#include <basic/meta/Meta.hpp>
#include <engine/vulqan/VqUtils.hpp>
#include <engine/vulqan/VqGLFW.hpp>
#include <engine/vulqan/VqInstance.hpp>
#include <engine/vulqan/VqWindow.hpp>
#include <iostream>

using namespace yq;

int main(int argc, char* argv[])
{
    BasicApp cargs(argc, argv);
    load_plugin_dir("plugin");
    Meta::freeze();
    
    VqGLFW                  glfw;
    
    VqInstance::Info        vi;
    vi.app_name     = "im_demo";
    vi.engine_name  = "YQ";
    
    VqInstance instance(vi);
    if(!instance.good())
        return 0;
    
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
