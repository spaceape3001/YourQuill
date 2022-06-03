////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ImWindow.hpp"
#include <basic/meta/ObjectInfoWriter.hpp>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>
#include <engine/vulqan/VqApp.hpp>
#include <engine/vulqan/VqStructs.hpp>

YQ_OBJECT_IMPLEMENT(yq::ImWindow)

namespace yq {
    ImWindow::ImWindow(const WindowCreateInfo& wci) : VqWindow(wci)
    {
        make_descriptors();
        
        m_imgui = ImGui::CreateContext();
        ImGui_ImplVulkan_InitInfo vii{};
        
        vii.Instance        = VqApp::instance();
        vii.PhysicalDevice  = physical();
        vii.Device          = device();
        vii.Queue           = graphics_queue();
        vii.QueueFamily     = graphics_queue_family();
        vii.MinImageCount   = swap_min_image_count();
        vii.ImageCount      = swap_image_count();
        vii.DescriptorPool  = m_descriptors;
        
        ImGui::SetCurrentContext(m_imgui);
        ImGui_ImplGlfw_InitForVulkan(window(), true);
        ImGui_ImplVulkan_Init(&vii, render_pass());
        
        upload_fonts();
    }
    
    ImWindow::~ImWindow()
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        if(m_imgui){
            ImGui::DestroyContext(m_imgui);
            m_imgui = nullptr;
        }
        if(m_descriptors){
            vkDestroyDescriptorPool(device(), m_descriptors, nullptr);
            m_descriptors   = nullptr;
        }
    }

    bool    ImWindow::draw() 
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        draw_imgui();
        ImGui::Render();
        if(!VqWindow::draw())
            return false;
        return true;
    }
    
    void    ImWindow::draw_vulqan(VkCommandBuffer buffer) 
    {
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), buffer, nullptr);
    }
    
    void    ImWindow::make_descriptors(uint32_t cnt)
    {
        std::vector<VkDescriptorPoolSize> pool_sizes =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, cnt },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, cnt },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, cnt },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, cnt },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, cnt },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, cnt },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, cnt },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, cnt },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, cnt },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, cnt },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, cnt }
        };
        VqDescriptorPoolCreateInfo pool_info;
        pool_info.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets       = cnt * pool_sizes.size();
        pool_info.poolSizeCount = (uint32_t) pool_sizes.size();
        pool_info.pPoolSizes    = pool_sizes.data();
        vkCreateDescriptorPool(device(), &pool_info, nullptr, &m_descriptors);
    }

    void    ImWindow::upload_fonts()
    {
        VkCommandBuffer cbuffer = command_buffer();

        vkResetCommandPool(device(), command_pool(), 0);
        VqCommandBufferBeginInfo begin_info;
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(cbuffer, &begin_info);

        ImGui_ImplVulkan_CreateFontsTexture(cbuffer);

        VqSubmitInfo end_info;
        end_info.commandBufferCount = 1;
        end_info.pCommandBuffers = &cbuffer;
        vkEndCommandBuffer(cbuffer);
        vkQueueSubmit(graphics_queue(), 1, &end_info, VK_NULL_HANDLE);
        vkDeviceWaitIdle(device());
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }
}
