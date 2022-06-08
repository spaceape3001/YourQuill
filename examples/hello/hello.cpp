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
#include <math/ColorRgb.hpp>
#include <math/vec/Vector2.hpp>
#include <iostream>

#include <glm/glm.hpp>  // temporary

using namespace yq;
using namespace yq::engine;

struct Vertex {
    glm::vec2    position;
    glm::vec3   color;
    
    
    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() 
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
        attributeDescriptions.resize(2);
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        return attributeDescriptions;
    }
};

const std::vector<Vertex> vertices = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};


struct HelloWin : public VqWindow {
    YQ_OBJECT_DECLARE(HelloWin, VqWindow)
    
    Pipeline                triangle;
    VkBuffer                vertexBuffer;
    VkDeviceMemory          vertexBufferMemory;
    
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

    HelloWin(const WindowCreateInfo& wci) : VqWindow(wci)
    {
        ShaderPtr   vert = Shader::load("examples/hello/hello2.vert");
        ShaderPtr   frag = Shader::load("examples/hello/hello.frag");
        if(!vert)
            throw std::runtime_error("No vertex shader");
        if(!frag)
            throw std::runtime_error("No fragment shader");
            

        PipelineConfig      cfg;
        cfg.shaders     = { vert, frag };
        cfg.front       = FrontFace::Clockwise;
        cfg.polymode    = PolygonMode::Fill;
        
        auto bindingDescription = Vertex::getBindingDescription();
        auto attributeDescriptions = Vertex::getAttributeDescriptions();
        
        if(!triangle.init(this, cfg, [&](VkPipelineVertexInputStateCreateInfo& vertexInfo){

            vertexInfo.vertexBindingDescriptionCount = 1;
            vertexInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
            vertexInfo.pVertexBindingDescriptions = &bindingDescription;
            vertexInfo.pVertexAttributeDescriptions = attributeDescriptions.data();            
        })){
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
    }
    
    ~HelloWin()
    {
        triangle.kill(this);

        vkDestroyBuffer(m_device, vertexBuffer, nullptr);
        vkFreeMemory(m_device, vertexBufferMemory, nullptr);
    }
    
    void        draw_vulqan(VkCommandBuffer cmdbuf)
    {
        vkCmdBindPipeline(cmdbuf, VK_PIPELINE_BIND_POINT_GRAPHICS, triangle.pipeline);
        
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
