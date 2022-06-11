////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ImWindow.hpp"
#include <basic/meta/ObjectInfoWriter.hpp>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>
#include <engine/Application.hpp>
#include <engine/vulqan/VqStructs.hpp>

YQ_OBJECT_IMPLEMENT(yq::engine::ImWindow)

namespace yq {
    namespace engine {
        ImWindow::ImWindow(const WindowCreateInfo& wci) : Window(wci)
        {
            m_imgui = ImGui::CreateContext();
            ImGui_ImplVulkan_InitInfo vii{};
            
            vii.Instance        = Application::vulkan();
            vii.PhysicalDevice  = physical();
            vii.Device          = device();
            vii.Queue           = graphics_queue();
            vii.QueueFamily     = graphics_queue_family();
            vii.MinImageCount   = swap_min_image_count();
            vii.ImageCount      = swap_image_count();
            vii.DescriptorPool  = descriptor_pool();
            
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
        }

        bool    ImWindow::draw() 
        {
            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            draw_imgui();
            ImGui::Render();
            if(!Window::draw())
                return false;
            return true;
        }
        
        void    ImWindow::draw_vulqan(VkCommandBuffer buffer) 
        {
            ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), buffer, nullptr);
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
}
