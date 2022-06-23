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

#include <basic/DelayInit.hpp>
#include <basic/Logging.hpp>
#include <basic/PluginLoader.hpp>
#include <basic/meta/Meta.hpp>
#include <engine/Application.hpp>
#include <engine/Vulqan.hpp>
#include <engine/PipelineBuilder.hpp>
#include <engine/Render3D.hpp>
#include <engine/Render3DInfoWriter.hpp>
#include <engine/Shader.hpp>
#include <engine/vulqan/VqBuffer.hpp>
#include <engine/vulqan/VqCommand.hpp>
#include <engine/vulqan/VqUtils.hpp>
#include <engine/vulqan/VqPipeline.hpp>
#include <math/RGB.hpp>
#include <math/Vector2.hpp>
#include <iostream>
#include <chrono>
#include <glm/glm.hpp>  // temporary
#include <math.h>

using namespace yq;
using namespace yq::engine;

struct Vertex {
    glm::vec2   position;
    glm::vec3   color;
};

struct Warp {
    float   amt = 0;
};

const Vertex vertices[] = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

using timepoint_t   = std::chrono::time_point<std::chrono::steady_clock>;

struct HelloTriangle : public engine::Rendered {
    YQ_OBJECT_DECLARE(HelloTriangle, engine::Rendered)
    VqPipeline                  m_pipeline;
    VqBuffer                    m_vbo;
    Vulqan*                     m_window                = nullptr;
    
    HelloTriangle(Vulqan*w) : m_window(w)
    {
        ShaderPtr   vert = Shader::load("examples/hello/hello3.vert");
        ShaderPtr   frag = Shader::load("examples/hello/hello.frag");
        if(!vert)
            throw std::runtime_error("No vertex shader");
        if(!frag)
            throw std::runtime_error("No fragment shader");
            

        PipelineBuilder      cfg;
        cfg.add_shaders({ vert, frag });
        cfg.front(FrontFace::Clockwise);
        cfg.push_constant<Warp>();
        
        cfg.vbo<Vertex>().attribute(&Vertex::position, DataFormat::R32G32_SFLOAT, 0).attribute(&Vertex::color, DataFormat::R32G32B32_SFLOAT, 1);
        
        m_pipeline    = VqPipeline(*w, cfg);
        if(!m_pipeline.good()){
            throw std::runtime_error("Unable to create pipeline!");
        }
        
        m_vbo   = VqBuffer(*w, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertices);
    }
    
    ~HelloTriangle()
    {
        m_pipeline  = {};
        m_vbo       = {};
    }
    
    void    render(VkCommandBuffer cmdbuf, double time)
    {
        uint32_t    sec = (int) time;
        bool        w = (sec & 1) != 0;
        Warp    warp { (float)( 1.0 + 0.5*sin(time)) };

        VkBuffer vertexBuffers[] = {m_vbo};
        VkDeviceSize offsets[] = {0};
    
        VqCommand       cmd;
        cmd.pipeline    = w ? m_pipeline.wireframe() : m_pipeline.pipeline();
        cmd.layout      = m_pipeline.layout();
        cmd.push        = warp; // VqCommand::Push{ &warp, sizeof(warp), m_pipeline.shader_mask() };
        cmd.push_mask   = m_pipeline.shader_mask();
        cmd.vbo         = VqCommand::VBO{ vertexBuffers, offsets, 1 };
        cmd.draw        = VqCommand::Draw{ 3 };
        record_draw(cmdbuf, cmd);
    }
};

YQ_INVOKE(
    [[maybe_unused]] auto ht   = writer<HelloTriangle>();
)

YQ_OBJECT_IMPLEMENT(HelloTriangle)


struct HelloWin : public engine::Vulqan {
    YQ_OBJECT_DECLARE(HelloWin, engine::Vulqan)
    
    timepoint_t             start;
    Ref<HelloTriangle>      triangle;

    HelloWin(const WindowCreateInfo& wci) : Vulqan(wci)
    {
        start   = std::chrono::steady_clock::now();
        triangle = new HelloTriangle(this);
    }
    
    ~HelloWin()
    {
        triangle    = nullptr;
    }
    
    void        draw_vulqan(VkCommandBuffer cmdbuf)
    {
        timepoint_t n   = std::chrono::steady_clock::now();
        std::chrono::duration<double>  diff    = start - n;
        triangle->render(cmdbuf, diff.count());
    }
};

YQ_OBJECT_IMPLEMENT(HelloWin)


int main(int argc, char* argv[])
{
    AppCreateInfo        vi;
    
    Application app(argc, argv, vi);
    //load_plugin_dir("plugin");
    app.finalize();
    
    WindowCreateInfo      wi;
    wi.title        = "Hello WORLD!";
    wi.resizable    = true;
    wi.size.x       = 1920;
    wi.clear        = { 0.f, 0.1f, 0.2f, 1.f };

    Ref<HelloWin>   win = new HelloWin(wi);
    if(!win->good())
        return -1;
    
    app.run_window(win.ptr());
    
    std::cout << "Hello World!\n";
    return 0;
}
