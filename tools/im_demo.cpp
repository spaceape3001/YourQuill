////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

/*
    Simple "demo" utilitiy
*/

#include <basic/CmdArgs.hpp>
#include <basic/Logging.hpp>
#include <basic/PluginLoader.hpp>
#include <basic/meta/Meta.hpp>
#include <engine/VqCore.hpp>
#include <engine/VqGLFW.hpp>
#include <engine/VqInstance.hpp>
#include <engine/VqWindow.hpp>
#include <iostream>

using namespace yq;

int main(int argc, char* argv[])
{
    CmdArgs cargs(argc, argv);
    log_to_std_error(LogPriority::Debug);
    Meta::init();
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
