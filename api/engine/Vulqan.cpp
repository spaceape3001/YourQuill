////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

/*
    YES, we're leveraging the "hello triangle" tutorial
    
    https://vulkan-tutorial.com/
*/

#include "Application.hpp"
#include "Vulqan.hpp"

#include <engine/vulqan/VqException.hpp>
#include <engine/vulqan/VqInternal.hpp>
#include <engine/vulqan/VqLogging.hpp>
#include <engine/vulqan/VqShaderStages.hpp>
#include <engine/vulqan/VqUtils.hpp>

#include <basic/CollectionUtils.hpp>
#include <basic/Logging.hpp>
#include <basic/Safety.hpp>
#include <basic/meta/ObjectInfoWriter.hpp>
#include <engine/PipelineConfig.hpp>
#include <math/Size2.hpp>
#include <math/Vector2.hpp>

#include <algorithm>
#include <cassert>

#include <GLFW/glfw3.h>

YQ_OBJECT_IMPLEMENT(yq::engine::Vulqan)

namespace yq {
    namespace engine {
        void Vulqan::poll_events()
        {
            glfwPollEvents();
        }
        
        WindowInfo::WindowInfo(std::string_view kname, ObjectInfo& base, const std::source_location& sl) :
            ObjectInfo(kname, base, sl)
        {
        }

        ////////////////////////////////////////////////////////////////////////////////

        Vulqan::Vulqan(const WindowCreateInfo&i)
        {
            try {
                m           = std::make_unique<VqInternal>(i,this);
            }
            catch(VqException& ex){
                yCritical() << ex.what();
            }
        }
        
        Vulqan::~Vulqan()
        {
            m           = nullptr;
        }



        ////////////////////////////////////////////////////////////////////////////////

        bool    Vulqan::record(VkCommandBuffer commandBuffer, uint32_t imageIndex)
        {
            VqCommandBufferBeginInfo beginInfo;
            beginInfo.flags = 0; // Optional
            beginInfo.pInheritanceInfo = nullptr; // Optional

            if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
                vqError << "Failed to begin recording command buffer!";
                return false;
            }

            VqRenderPassBeginInfo renderPassInfo;
            renderPassInfo.renderPass = m->renderPass;
            renderPassInfo.framebuffer = m->dynamic.frameBuffers[imageIndex];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = m->dynamic.swapchain.extents();

            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &m->clear;
            vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        
            draw_vulqan(commandBuffer);

            vkCmdEndRenderPass(commandBuffer);

            if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
                vqError << "Failed to record command buffer!";
                return false;
            }
            return true;
        }
        
        bool    Vulqan::draw()
        {
            bool    rebuild = m->rebuildSwap.exchange(false);
            if(rebuild){
                vkDeviceWaitIdle(m->device);
                VqDynamic        newStuff;
                m->init(newStuff, m->dynamic.swapchain);
                std::swap(m->dynamic, newStuff);
                m->kill(newStuff);
                
                //  resize notifications...
            }
            
            VkCommandBuffer cmdbuf = m->dynamic.commandBuffers[0];
            
        
            m->inFlightFence.wait_reset();
            uint32_t imageIndex = 0;
            vkAcquireNextImageKHR(m->device, m->dynamic.swapchain, UINT64_MAX, m->imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
            vkResetCommandBuffer(cmdbuf, 0);
            
            if(!record(cmdbuf, imageIndex))
                return false;
            
            VqSubmitInfo submitInfo;

            VkSemaphore waitSemaphores[] = {m->imageAvailableSemaphore};
            VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores      = waitSemaphores;
            submitInfo.pWaitDstStageMask    = waitStages;
            submitInfo.commandBufferCount   = (uint32_t) m->dynamic.commandBuffers.size();
            submitInfo.pCommandBuffers      = m->dynamic.commandBuffers.data();

            VkSemaphore signalSemaphores[] = {m->renderFinishedSemaphore};
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = signalSemaphores;

            if (vkQueueSubmit(m->device.graphics(0), 1, &submitInfo, m->inFlightFence) != VK_SUCCESS) {
                vqError << "Failed to submit draw command buffer!";
                return false;
            }
                
            VqPresentInfoKHR presentInfo;
            
            VkSwapchainKHR      swapchain   = m->dynamic.swapchain;

            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = signalSemaphores;
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = &swapchain;
            presentInfo.pImageIndices = &imageIndex;
            presentInfo.pResults = nullptr; // Optional
            vkQueuePresentKHR(m->device.present(0), &presentInfo);
            return true;
        }
        
        
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////

        void Vulqan::attention()
        {
            if(m->window)
                glfwRequestWindowAttention(m->window);
        }

        void Vulqan::close()
        {
            if(m->window)
                glfwSetWindowShouldClose(m->window, GLFW_TRUE);
        }

        VkColorSpaceKHR     Vulqan::color_space() const 
        { 
            return m->surface.color_space(); 
        }

        VkCommandBuffer     Vulqan::command_buffer() const
        {
            return m->dynamic.commandBuffers[0];
        }

        VkCommandPool       Vulqan::command_pool() const 
        { 
            return m->commandPool; 
        }
        
        VkDescriptorPool    Vulqan::descriptor_pool() const 
        { 
            return m->descriptorPool; 
        }
        
        VkDevice            Vulqan::device() const 
        { 
            return m->device; 
        }

        void Vulqan::focus()
        {
            if(m->window)
                glfwFocusWindow(m->window);
        }

        VkFormat    Vulqan::format() const 
        { 
            return m->surface.format(); 
        }

        VkQueue     Vulqan::graphics_queue() const 
        { 
            return m->device.graphics(0); 
        }
        
        uint32_t    Vulqan::graphics_queue_family() const 
        { 
            return m->device.graphics().family(); 
        }

        int  Vulqan::height() const
        {
            if(!m->window)
                return 0;
        
            int ret;
            glfwGetWindowSize(m->window, nullptr, &ret);
            return ret;
        }

        void Vulqan::hide()
        {
            if(m->window)
                glfwHideWindow(m->window);
        }

        void Vulqan::iconify()
        {
            if(m->window)
                glfwIconifyWindow(m->window);
        }

        bool        Vulqan::is_decorated() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowAttrib(m->window, GLFW_DECORATED) != 0;
        }
        
        bool        Vulqan::is_focused() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowAttrib(m->window, GLFW_FOCUSED ) != 0;
        }
        
        bool        Vulqan::is_floating() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowAttrib(m->window, GLFW_FLOATING) != 0;
        }
        
        bool        Vulqan::is_fullscreen() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowMonitor(m->window) != nullptr;
        }
        
        bool        Vulqan::is_hovered() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowAttrib(m->window, GLFW_HOVERED) != 0;
        }
        
        bool        Vulqan::is_iconified() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowAttrib(m->window, GLFW_ICONIFIED) != 0;
        }

        bool        Vulqan::is_maximized() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowAttrib(m->window, GLFW_MAXIMIZED) != 0;
        }
        
        bool        Vulqan::is_resizable() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowAttrib(m->window, GLFW_RESIZABLE) != 0;
        }
        
        bool        Vulqan::is_visible() const
        {
            if(!m->window)
                return false;
            return glfwGetWindowAttrib(m->window, GLFW_VISIBLE) != 0;
        }
        
        VkDevice    Vulqan::logical() const 
        { 
            return m->device; 
        }

        void        Vulqan::maximize()
        {
            if(m->window)
                glfwMaximizeWindow(m->window);
        }

        VqMonitor   Vulqan::monitor() const
        {
            if(m->window)
                return VqMonitor(glfwGetWindowMonitor(m->window));
            return VqMonitor();
        }

        VkPhysicalDevice    Vulqan::physical() const 
        { 
            return m->physical; 
        }

        Vector2I    Vulqan::position() const
        {
            if(!m->window)
                return {};
            Vector2I   ret;
            glfwGetWindowPos(m->window, &ret.x, &ret.y);
            return ret;
        }


        VkRenderPass Vulqan::render_pass() const
        {
            return m->renderPass;
        }

        void        Vulqan::restore()
        {
            if(m->window)
                glfwRestoreWindow(m->window);
        }

        void        Vulqan::set_clear(const RGBA4F&i)
        {
            m->set_clear(i);
        }

        void        Vulqan::set_position(const Vector2I& pos)
        {
            set_position(pos.x, pos.y);
        }
        
        void        Vulqan::set_position(int x, int y)
        {
            if(m->window){
                glfwSetWindowPos(m->window, x, y);
            }
        }

        void        Vulqan::set_size(const Size2I& sz)
        {
            set_size(sz.x, sz.y);
        }

        void        Vulqan::set_size(int w, int h)
        {
            if(m->window){
                glfwSetWindowSize(m->window, std::max(1, w), std::max(1, h));
            }
        }

        void        Vulqan::set_title(const char*z)
        {
            if(m->window && z){
                glfwSetWindowTitle(m->window, z);
            }
        }

        void        Vulqan::set_title(const std::string&z)
        {
            set_title(z.c_str());
        }

        bool        Vulqan::should_close() const
        {
            if(!m->window) 
                return true;
            return glfwWindowShouldClose(m->window);
        }

        void        Vulqan::show()
        {
            if(m->window)
                glfwShowWindow(m->window);
        }

        Size2I      Vulqan::size() const
        {
            if(!m->window)
                return {};
            Size2I  ret;
            glfwGetWindowSize(m->window, &ret.x, &ret.y);
            return ret;
        }

        VkSurfaceKHR        Vulqan::surface() const 
        { 
            return m->surface; 
        }

        VkRect2D    Vulqan::swap_def_scissor() const
        {
            return m->dynamic.swapchain.def_scissor();
        }
        
        VkViewport  Vulqan::swap_def_viewport() const
        {
            return m->dynamic.swapchain.def_viewport();
        }

        uint32_t    Vulqan::swap_image_count() const
        {
            return m->dynamic.imageCount;
        }

        uint32_t    Vulqan::swap_height() const
        {
            return m->dynamic.swapchain.height();
        }

        uint32_t    Vulqan::swap_min_image_count() const
        {
            return m->dynamic.swapchain.min_image_count();
        }

        uint32_t    Vulqan::swap_width() const
        {
            return m->dynamic.swapchain.width();
        }
        
        int  Vulqan::width() const
        {
            if(!m->window)
                return 0;
        
            int ret;
            glfwGetWindowSize(m->window, &ret, nullptr);
            return ret;
        }
        
        GLFWwindow*         Vulqan::window() const 
        { 
            return m->window; 
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////

        YQ_INVOKE(
            writer<Vulqan>();
        )
    }
}
