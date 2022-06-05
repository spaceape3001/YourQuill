////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

/*

    This example is about demonstrating the use of the API to display a 
    simple triangle (in 2D mode) statically on the window.
    
    Note, it's heavily leveraged/inspired/taken from the Hello Triangle SDK example 
    located at https://vulkan-tutorial.com/Introduction
*/

//  Also it's about me developing the API in the first place .... :)


#include <basic/Logging.hpp>
#include <basic/PluginLoader.hpp>
#include <basic/meta/Meta.hpp>
#include <engine/render/PipelineConfig.hpp>
#include <engine/shader/Shader.hpp>
#include <engine/vulqan/VqApp.hpp>
#include <engine/vulqan/VqUtils.hpp>
#include <engine/vulqan/VqWindow.hpp>
#include <iostream>

using namespace yq;
using namespace yq::engine;

struct HelloWin : public VqWindow {
    YQ_OBJECT_DECLARE(HelloWin, VqWindow)
    
    Pipeline                triangle;
    
    HelloWin(const WindowCreateInfo& wci) : VqWindow(wci)
    {
        ShaderPtr   vert = Shader::load("examples/hello/hello.vert");
        ShaderPtr   frag = Shader::load("examples/hello/hello.frag");
        if(!vert)
            throw std::runtime_error("No vertex shader");
        if(!frag)
            throw std::runtime_error("No fragment shader");
            

        PipelineConfig      cfg;
        cfg.shaders     = { vert, frag };
        cfg.front       = FrontFace::Clockwise;
        cfg.polymode    = PolygonMode::Fill;
        if(!triangle.init(this, cfg))
            throw std::runtime_error("Unable to create pipeline!");
    }
    
    ~HelloWin()
    {
        triangle.kill(this);
    }
    
    void        draw_vulqan(VkCommandBuffer cmdbuf)
    {
        vkCmdBindPipeline(cmdbuf, VK_PIPELINE_BIND_POINT_GRAPHICS, triangle.pipeline);
        vkCmdDraw(cmdbuf, 3, 1, 0, 0);
    }
};

YQ_OBJECT_IMPLEMENT(HelloWin)


int main(int argc, char* argv[])
{
    AppCreateInfo        vi;
    
    VqApp app(argc, argv, vi);
    //load_plugin_dir("plugin");
    app.finalize();
    
    WindowCreateInfo      wi;
    wi.title        = "Hello WORLD!";
    wi.resizable    = true;
    wi.size.x       = 1920;
    wi.clear        = { 0.f, 0.f, 0.5f, 1.f };

    Ref<HelloWin>   win = new HelloWin(wi);
    app.run_window(win.ptr());
    
    std::cout << "Hello World!\n";
    return 0;
}
