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
#include <engine/Application.hpp>
#include <engine/Window.hpp>
#include <engine/pipeline/PipelineBuilder.hpp>
#include <engine/shader/Shader.hpp>
#include <engine/vulqan/VqUtils.hpp>
#include <math/ColorRgb.hpp>
#include <math/vec/Vector2.hpp>
#include <iostream>
#include <chrono>
#include <glm/glm.hpp>  // temporary

using namespace yq;
using namespace yq::engine;

struct Vertex {
    glm::vec2   position;
    glm::vec3   color;
};

const std::vector<Vertex> vertices = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

using timepoint_t   = std::chrono::time_point<std::chrono::steady_clock>;


struct HelloWin : public Window {
    YQ_OBJECT_DECLARE(HelloWin, Window)
    
    Pipeline                triangle;
    VkBuffer                vertexBuffer;
    VkDeviceMemory          vertexBufferMemory;
    timepoint_t             start;
    
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) 
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(m_physical, &memProperties);
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }

    HelloWin(const WindowCreateInfo& wci) : Window(wci)
    {
        ShaderPtr   vert = Shader::load("examples/hello/hello2.vert");
        ShaderPtr   frag = Shader::load("examples/hello/hello.frag");
        if(!vert)
            throw std::runtime_error("No vertex shader");
        if(!frag)
            throw std::runtime_error("No fragment shader");
            

        PipelineBuilder      cfg;
        cfg.add_shaders({ vert, frag });
        cfg.front(FrontFace::Clockwise);
        
        cfg.vbo<Vertex>().attribute(&Vertex::position, DataFormat::R32G32_SFLOAT, 0).attribute(&Vertex::color, DataFormat::R32G32B32_SFLOAT, 1);
        
        if(!triangle.init(this, cfg)){
            throw std::runtime_error("Unable to create pipeline!");
        }
            
        VqBufferCreateInfo  bufferInfo;
        bufferInfo.size = sizeof(vertices[0]) * vertices.size();
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(m_device, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS){
            throw std::runtime_error("Unable to create vertex buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(m_device, vertexBuffer, &memRequirements);

        VqMemoryAllocateInfo allocInfo;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(m_device, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        }        

        vkBindBufferMemory(m_device, vertexBuffer, vertexBufferMemory, 0);
        void* data;
        vkMapMemory(m_device, vertexBufferMemory, 0, bufferInfo.size, 0, &data);
        memcpy(data, vertices.data(), (size_t) bufferInfo.size);
        vkUnmapMemory(m_device, vertexBufferMemory);
        
        start   = std::chrono::steady_clock::now();
    }
    
    ~HelloWin()
    {
        triangle.kill(this);

        vkDestroyBuffer(m_device, vertexBuffer, nullptr);
        vkFreeMemory(m_device, vertexBufferMemory, nullptr);
    }
    
    void        draw_vulqan(VkCommandBuffer cmdbuf)
    {
        timepoint_t n   = std::chrono::steady_clock::now();
        std::chrono::duration<double>  diff    = start - n;
        uint32_t    sec = (int) diff.count();
        bool        w = (sec & 1) != 0;
    
        vkCmdBindPipeline(cmdbuf, VK_PIPELINE_BIND_POINT_GRAPHICS, w ? triangle.wireframe : triangle.pipeline);
        
        VkBuffer vertexBuffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(cmdbuf, 0, 1, vertexBuffers, offsets);        
        
        vkCmdDraw(cmdbuf, 3, 1, 0, 0);
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
