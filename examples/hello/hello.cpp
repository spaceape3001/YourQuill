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
#include <engine/Window.hpp>
#include <engine/pipeline/PipelineBuilder.hpp>
#include <engine/render/RenderObject.hpp>
#include <engine/render/RenderObjectInfoWriter.hpp>
#include <engine/shader/Shader.hpp>
#include <engine/vulqan/VqCommand.hpp>
#include <engine/vulqan/VqUtils.hpp>
#include <engine/vulqan/VqPipeline.hpp>
#include <math/RGB.hpp>
#include <math/vec/Vector2.hpp>
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

struct HelloTriangle : public Renderable {
    YQ_OBJECT_DECLARE(HelloTriangle, Renderable)

    std::span<const Vertex>     m_vbo;
    VqPipeline                  m_pipeline;
    VkBuffer                    m_vertexBuffer          = nullptr;
    VkDeviceMemory              m_vertexBufferMemory    = nullptr;
    Window*                     m_window                = nullptr;

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) 
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(m_window->physical(), &memProperties);
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }
    
    HelloTriangle(Window*w) : m_window(w)
    {
        m_vbo     = std::span<const Vertex>(vertices, sizeof(vertices)/sizeof(Vertex));
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
        
        m_pipeline    = VqPipeline(m_window, cfg);
        if(!m_pipeline.good()){
            throw std::runtime_error("Unable to create pipeline!");
        }
            
        VqBufferCreateInfo  bufferInfo;
        bufferInfo.size = sizeof(Vertex) * m_vbo.size();
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(w->device(), &bufferInfo, nullptr, &m_vertexBuffer) != VK_SUCCESS){
            throw std::runtime_error("Unable to create vertex buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(w->device(), m_vertexBuffer, &memRequirements);

        VqMemoryAllocateInfo allocInfo;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(w->device(), &allocInfo, nullptr, &m_vertexBufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        }        

        vkBindBufferMemory(w->device(), m_vertexBuffer, m_vertexBufferMemory, 0);
        void* data;
        vkMapMemory(w->device(), m_vertexBufferMemory, 0, bufferInfo.size, 0, &data);
        memcpy(data, m_vbo.data(), (size_t) bufferInfo.size);
        vkUnmapMemory(w->device(), m_vertexBufferMemory);
    }
    
    ~HelloTriangle()
    {
        m_pipeline  = {};
        vkDestroyBuffer(m_window->device(), m_vertexBuffer, nullptr);
        vkFreeMemory(m_window->device(), m_vertexBufferMemory, nullptr);
    }
    
    void    render(VkCommandBuffer cmdbuf, double time)
    {
        uint32_t    sec = (int) time;
        bool        w = (sec & 1) != 0;
        Warp    warp { (float)( 1.0 + 0.5*sin(time)) };

        VkBuffer vertexBuffers[] = {m_vertexBuffer};
        VkDeviceSize offsets[] = {0};
    
        VqCommand       cmd;
        cmd.pipeline    = w ? m_pipeline.wireframe() : m_pipeline.pipeline();
        cmd.layout      = m_pipeline.layout();
        cmd.push        = VqCommand::Push{ &warp, sizeof(warp), m_pipeline.shader_mask() };
        cmd.vbo         = VqCommand::VBO{ vertexBuffers, offsets, 1 };
        cmd.draw        = VqCommand::Draw{ 3 };
        record_draw(cmdbuf, cmd);
    }
};

YQ_INVOKE(
    [[maybe_unused]] auto ht   = writer<HelloTriangle>();
)

YQ_OBJECT_IMPLEMENT(HelloTriangle)


struct HelloWin : public Window {
    YQ_OBJECT_DECLARE(HelloWin, Window)
    
    timepoint_t             start;
    HelloTriangle*          triangle = nullptr;

    HelloWin(const WindowCreateInfo& wci) : Window(wci)
    {
        start   = std::chrono::steady_clock::now();
        triangle = new HelloTriangle(this);
    }
    
    ~HelloWin()
    {
        delete triangle;
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
    wi.clear        = { 0.f, 0.f, 0.5f, 1.f };

    Ref<HelloWin>   win = new HelloWin(wi);
    if(!win->valid())
        return -1;
    
    app.run_window(win.ptr());
    
    std::cout << "Hello World!\n";
    return 0;
}
