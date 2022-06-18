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
#include "Window.hpp"

#include <engine/vulqan/VqException.hpp>
#include <engine/vulqan/VqLogging.hpp>
#include <engine/vulqan/VqShaderStages.hpp>
#include <engine/vulqan/VqUtils.hpp>

#include <basic/CollectionUtils.hpp>
#include <basic/Logging.hpp>
#include <basic/Safety.hpp>
#include <basic/meta/ObjectInfoWriter.hpp>
#include <engine/pipeline/PipelineConfig.hpp>
#include <math/shape/Size2.hpp>
#include <math/vec/Vector2.hpp>

#include <algorithm>
#include <cassert>

#include <GLFW/glfw3.h>

YQ_OBJECT_IMPLEMENT(yq::engine::Window)

namespace yq {
    namespace engine {
        void Window::poll_events()
        {
            glfwPollEvents();
        }
        
        WindowInfo::WindowInfo(std::string_view kname, ObjectInfo& base, const std::source_location& sl) :
            ObjectInfo(kname, base, sl)
        {
        }

        ////////////////////////////////////////////////////////////////////////////////

        void Window::callback_resize(GLFWwindow* gwin, int, int)
        {
            Window    *v  = (Window*) glfwGetWindowUserPointer(gwin);
            if(v)
                v -> m_rebuildSwap    = true;
        }

        ////////////////////////////////////////////////////////////////////////////////

        Window::Window(const WindowCreateInfo&i)
        {
            try {
                VkInstance     inst  = Application::vulkan();
                if(!inst)
                    throw VqException("Vulkan has not been initialized!");
                
                
                VkPhysicalDevice    gpu = i.device;
                gpu  = i.device;
                if(!gpu){
                    gpu  = vqFirstDevice();
                    if(!gpu)
                        throw VqException("Cannot create window without any devices!");
                }

                m_physical                  = VqGPU(gpu);
                yNotice() << "Using (" << to_string(m_physical.device_type()) << "): " << m_physical.device_name();

                m_window                    = VqWindow(this, i);
                m_surface                   = VqSurface(m_physical, m_window);
                
                VqDevice::Config    dci;
                dci.extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
                m_device                    = VqDevice(m_surface, dci);

           
                m_presentMode               = m_surface.supports(i.pmode) ? i.pmode : VK_PRESENT_MODE_FIFO_KHR;
                    
                m_commandPool               = VqCommandPool(m_device, m_device.graphics().family());
                m_renderPass                = VqRenderPass(this);
                m_imageAvailableSemaphore   = VqSemaphore(m_device);
                m_renderFinishedSemaphore   = VqSemaphore(m_device);
                m_inFlightFence             = VqFence(m_device);
                m_descriptorPool            = VqDescriptorPool(m_device, i.descriptors);

                if(!init(m_dynamic))
                    throw VqException("");
                
                set_clear(i.clear);
                return;
            } 
            catch(VqException& ex){
                yCritical() << ex.what();
            }
            kill();
        }
        
        Window::~Window()
        {
            kill();
        }

        bool Window::init(DynamicStuff&ds, VkSwapchainKHR old)
        {
            VqSwapchain::Config scfg;
            scfg.pmode      = m_presentMode;
            scfg.old        = old;
            ds.swapchain    = VqSwapchain(m_device, m_surface, scfg);
            
            ds.images       = ds.swapchain.images();
            ds.imageCount   = ds.images.size();
            ds.imageViews       = VqImageViews(m_device, m_surface, ds.images);
            ds.frameBuffers     = VqFrameBuffers(m_device, m_renderPass, ds.swapchain.extents(), ds.imageViews);
            ds.commandBuffers   = VqCommandBuffers(m_commandPool, 1);

            return true;            
        }


        void Window::kill(DynamicStuff&ds)
        {
            ds.commandBuffers  = {};
            ds.frameBuffers     = {};
            ds.imageViews       = {};
            ds.swapchain        = {};
        }

        void Window::kill()
        {
            if(m_device){
                vkDeviceWaitIdle(m_device);
            }
            
            kill(m_dynamic);
            
            m_descriptorPool            = {};
            m_imageAvailableSemaphore   = {};
            m_renderFinishedSemaphore   = {};
            m_inFlightFence             = {};
            m_renderPass                = {};
            m_commandPool               = {};
            m_device                    = {};
            
            m_surface                   = {};
            m_window                    = {};
            
            m_physical = {};
        }


        
        ////////////////////////////////////////////////////////////////////////////////

        bool    Window::record(VkCommandBuffer commandBuffer, uint32_t imageIndex)
        {
            VqCommandBufferBeginInfo beginInfo;
            beginInfo.flags = 0; // Optional
            beginInfo.pInheritanceInfo = nullptr; // Optional

            if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
                vqError << "Failed to begin recording command buffer!";
                return false;
            }

            VqRenderPassBeginInfo renderPassInfo;
            renderPassInfo.renderPass = m_renderPass;
            renderPassInfo.framebuffer = m_dynamic.frameBuffers[imageIndex];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = m_dynamic.swapchain.extents();

            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &m_clear;
            vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        
            draw_vulqan(commandBuffer);

            vkCmdEndRenderPass(commandBuffer);

            if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
                vqError << "Failed to record command buffer!";
                return false;
            }
            return true;
        }
        
        bool    Window::draw()
        {
            bool    rebuild = m_rebuildSwap.exchange(false);
            if(rebuild){
                vkDeviceWaitIdle(m_device);
                DynamicStuff        newStuff;
                init(newStuff, m_dynamic.swapchain);
                std::swap(m_dynamic, newStuff);
                kill(newStuff);
                
                //  resize notifications...
            }
            
            VkCommandBuffer cmdbuf = m_dynamic.commandBuffers[0];
            
        
            m_inFlightFence.wait_reset();
            uint32_t imageIndex = 0;
            vkAcquireNextImageKHR(m_device, m_dynamic.swapchain, UINT64_MAX, m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
            vkResetCommandBuffer(cmdbuf, 0);
            
            if(!record(cmdbuf, imageIndex))
                return false;
            
            VqSubmitInfo submitInfo;

            VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphore};
            VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores      = waitSemaphores;
            submitInfo.pWaitDstStageMask    = waitStages;
            submitInfo.commandBufferCount   = (uint32_t) m_dynamic.commandBuffers.size();
            submitInfo.pCommandBuffers      = m_dynamic.commandBuffers.data();

            VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphore};
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = signalSemaphores;

            if (vkQueueSubmit(m_device.graphics(0), 1, &submitInfo, m_inFlightFence) != VK_SUCCESS) {
                vqError << "Failed to submit draw command buffer!";
                return false;
            }
                
            VqPresentInfoKHR presentInfo;
            
            VkSwapchainKHR      swapchain   = m_dynamic.swapchain;

            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = signalSemaphores;
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = &swapchain;
            presentInfo.pImageIndices = &imageIndex;
            presentInfo.pResults = nullptr; // Optional
            vkQueuePresentKHR(m_device.present(0), &presentInfo);
            return true;
        }
        
        
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////

        void Window::attention()
        {
            if(m_window)
                glfwRequestWindowAttention(m_window);
        }

        void Window::close()
        {
            if(m_window)
                glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        }

        VkCommandBuffer     Window::command_buffer() const
        {
            return m_dynamic.commandBuffers[0];
        }

        void Window::focus()
        {
            if(m_window)
                glfwFocusWindow(m_window);
        }

        VkQueue     Window::graphics_queue() const 
        { 
            return m_device.graphics(0); 
        }
        
        uint32_t    Window::graphics_queue_family() const 
        { 
            return m_device.graphics().family(); 
        }

        int  Window::height() const
        {
            if(!m_window)
                return 0;
        
            int ret;
            glfwGetWindowSize(m_window, nullptr, &ret);
            return ret;
        }

        void Window::hide()
        {
            if(m_window)
                glfwHideWindow(m_window);
        }

        void Window::iconify()
        {
            if(m_window)
                glfwIconifyWindow(m_window);
        }

        bool        Window::is_decorated() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_DECORATED) != 0;
        }
        
        bool        Window::is_focused() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_FOCUSED ) != 0;
        }
        
        bool        Window::is_floating() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_FLOATING) != 0;
        }
        
        bool        Window::is_fullscreen() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowMonitor(m_window) != nullptr;
        }
        
        bool        Window::is_hovered() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_HOVERED) != 0;
        }
        
        bool        Window::is_iconified() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0;
        }

        bool        Window::is_maximized() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED) != 0;
        }
        
        bool        Window::is_resizable() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_RESIZABLE) != 0;
        }
        
        bool        Window::is_visible() const
        {
            if(!m_window)
                return false;
            return glfwGetWindowAttrib(m_window, GLFW_VISIBLE) != 0;
        }
        

        void        Window::maximize()
        {
            if(m_window)
                glfwMaximizeWindow(m_window);
        }

        VqMonitor   Window::monitor() const
        {
            if(m_window)
                return VqMonitor(glfwGetWindowMonitor(m_window));
            return VqMonitor();
        }

        Vector2I    Window::position() const
        {
            if(!m_window)
                return {};
            Vector2I   ret;
            glfwGetWindowPos(m_window, &ret.x, &ret.y);
            return ret;
        }


        VkRenderPass Window::render_pass() const
        {
            return m_renderPass;
        }

        void        Window::restore()
        {
            if(m_window)
                glfwRestoreWindow(m_window);
        }

        void        Window::set_clear(const RGBA4F&i)
        {
            m_clear = VkClearValue{{{ i.red, i.green, i.blue, i.alpha }}};
        }

        void        Window::set_position(const Vector2I& pos)
        {
            set_position(pos.x, pos.y);
        }
        
        void        Window::set_position(int x, int y)
        {
            if(m_window){
                glfwSetWindowPos(m_window, x, y);
            }
        }

        void        Window::set_size(const Size2I& sz)
        {
            set_size(sz.x, sz.y);
        }

        void        Window::set_size(int w, int h)
        {
            if(m_window){
                glfwSetWindowSize(m_window, std::max(1, w), std::max(1, h));
            }
        }

        void        Window::set_title(const char*z)
        {
            if(m_window && z){
                glfwSetWindowTitle(m_window, z);
            }
        }

        void        Window::set_title(const std::string&z)
        {
            set_title(z.c_str());
        }

        bool        Window::should_close() const
        {
            if(!m_window) 
                return true;
            return glfwWindowShouldClose(m_window);
        }

        void        Window::show()
        {
            if(m_window)
                glfwShowWindow(m_window);
        }

        Size2I      Window::size() const
        {
            if(!m_window)
                return {};
            Size2I  ret;
            glfwGetWindowSize(m_window, &ret.x, &ret.y);
            return ret;
        }

        VkRect2D    Window::swap_def_scissor() const
        {
            return m_dynamic.swapchain.def_scissor();
        }
        
        VkViewport  Window::swap_def_viewport() const
        {
            return m_dynamic.swapchain.def_viewport();
        }

        uint32_t    Window::swap_image_count() const
        {
            return m_dynamic.imageCount;
        }

        uint32_t    Window::swap_height() const
        {
            return m_dynamic.swapchain.height();
        }

        uint32_t    Window::swap_min_image_count() const
        {
            return m_dynamic.swapchain.min_image_count();
        }

        uint32_t    Window::swap_width() const
        {
            return m_dynamic.swapchain.width();
        }
        
        int  Window::width() const
        {
            if(!m_window)
                return 0;
        
            int ret;
            glfwGetWindowSize(m_window, &ret, nullptr);
            return ret;
        }
        
        
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////

        YQ_INVOKE(
            writer<Window>();
        )
    }

}
