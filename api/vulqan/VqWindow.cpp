////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqWindow.hpp"
#include "VqCore.hpp"
#include "VqInstance.hpp"
#include "VqGLFW.hpp"

#include <basic/Logging.hpp>
#include <math/shape/Size2.hpp>
#include <math/vec/Vec2.hpp>

#include <cassert>

namespace yq {
    void VqWindow::poll_events()
    {
        glfwPollEvents();
    }
    
    ////////////////////////////////////////////////////////////////////////////////

    VqWindow::VqWindow(const Info&i)
    {
        _init(i);
    }
    
    VqWindow::~VqWindow()
    {
        _deinit();
    }

    void VqWindow::_deinit()
    {
        if(m_logical){
            vkDestroyDevice(m_logical, nullptr);
            m_logical   = nullptr;
        }

        if(m_surface){
            vkDestroySurfaceKHR(VqInstance::vulkan(), m_surface, nullptr);
            m_surface  = nullptr;
        }

        if(m_window){
            glfwDestroyWindow(m_window);
            m_window    = nullptr;
        }
        
        m_physical = nullptr;
    }
    
    bool VqWindow::_init(const Info& i)
    {
        const VqInstance*   inst    = VqInstance::singleton();
        if(!inst){
            yCritical() << "Vulkan has not been initialized!";
            return false;
        }
    
        m_physical    = i.device;
        if(!m_physical)
            m_physical  = vqFirstDevice();
        if(!m_physical){
            yCritical() << "Cannot create window without any devices!";
            return false;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_FLOATING, i.floating ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, i.decorated ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, i.resizable ? GLFW_TRUE : GLFW_FALSE);

        m_window = glfwCreateWindow(std::max(1,i.size.width()), std::max(1,i.size.height()), i.title, i.monitor.monitor(), nullptr);
        if(!m_window){
            yError() << "Unable to create window.";
            return false;
        }
        
        glfwSetWindowUserPointer(m_window, this);
        
            /*
                Create our surface
            */
        
        if(glfwCreateWindowSurface(inst->m_instance, m_window, nullptr, &m_surface) != VK_SUCCESS){
            yCritical() << "Unable to create window surface!";
            return false;
        }

        /*
            Create our logical device
        */
        
        std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    
        auto queueInfos         = vqGetPhysicalDeviceQueueFamilyProperties(m_physical);
        Result<uint32_t> gqu    = vqFindFirstGraphicsQueue(queueInfos);
        if(!gqu.good){
            yCritical() << "Unable to get a queue with graphic capability!";
            _deinit();
            return false;
        }
    
        Result<uint32_t>    pqu;
        for(uint32_t i=0;i<(uint32_t) queueInfos.size();++i){
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(m_physical, i, m_surface, &presentSupport);
            if(presentSupport){
                pqu = { i, true };
                break;
            }
        }
    
        if(!pqu.good){
            yCritical() << "Unable to find a present queue!";
            _deinit();
            return false;
        }
    
        std::vector<VkDeviceQueueCreateInfo> qci;
        float quePri    = 1.0f;
        
        for(uint32_t i : std::set<uint32_t>({ gqu.value, pqu.value})){
            VqDeviceQueueCreateInfo info;
            info.queueFamilyIndex = i;
            info.queueCount = 1;
            info.pQueuePriorities    = &quePri;
            qci.push_back(info);
        }
    
        VkPhysicalDeviceFeatures    df{};
        VqDeviceCreateInfo          dci;
        dci.pQueueCreateInfos        = qci.data();
        dci.queueCreateInfoCount     = (uint32_t) qci.size();
        dci.pEnabledFeatures         = &df;
        
        dci.enabledLayerCount        = (uint32_t) inst->m_layers.size();
        if(dci.enabledLayerCount)
            dci.ppEnabledLayerNames  = inst->m_layers.data();
    
        dci.enabledExtensionCount    = (uint32_t) deviceExtensions.size();
        if(!deviceExtensions.empty())
            dci.ppEnabledExtensionNames = deviceExtensions.data();
        
        if(vkCreateDevice(m_physical, &dci, nullptr, &m_logical) != VK_SUCCESS){
            yCritical() << "Unable to create logical device!";
            _deinit();
            return false;
        }
        
            /*
                Graphics queue
            */
        vkGetDeviceQueue(m_logical, gqu.value, 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_logical, pqu.value, 0, &m_presentQueue);
        
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical, m_surface, &m_capabilities);
    
        return true;
    }

    
    ////////////////////////////////////////////////////////////////////////////////

    void VqWindow::attention()
    {
        if(m_window)
            glfwRequestWindowAttention(m_window);
    }

    void VqWindow::close()
    {
        if(m_window)
            glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }

    void VqWindow::focus()
    {
        if(m_window)
            glfwFocusWindow(m_window);
    }

    int  VqWindow::height() const
    {
        if(!m_window)
            return 0;
    
        int ret;
        glfwGetWindowSize(m_window, nullptr, &ret);
        return ret;
    }

    void VqWindow::hide()
    {
        if(m_window)
            glfwHideWindow(m_window);
    }

    void VqWindow::iconify()
    {
        if(m_window)
            glfwIconifyWindow(m_window);
    }

    bool        VqWindow::is_decorated() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowAttrib(m_window, GLFW_DECORATED) != 0;
    }
    
    bool        VqWindow::is_focused() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowAttrib(m_window, GLFW_FOCUSED ) != 0;
    }
    
    bool        VqWindow::is_floating() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowAttrib(m_window, GLFW_FLOATING) != 0;
    }
    
    bool        VqWindow::is_fullscreen() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowMonitor(m_window) != nullptr;
    }
    
    bool        VqWindow::is_hovered() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowAttrib(m_window, GLFW_HOVERED) != 0;
    }
    
    bool        VqWindow::is_iconified() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0;
    }

    bool        VqWindow::is_maximized() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED) != 0;
    }
    
    bool        VqWindow::is_resizable() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowAttrib(m_window, GLFW_RESIZABLE) != 0;
    }
    
    bool        VqWindow::is_visible() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowAttrib(m_window, GLFW_VISIBLE) != 0;
    }
    

    void        VqWindow::maximize()
    {
        if(m_window)
            glfwMaximizeWindow(m_window);
    }

    VqMonitor   VqWindow::monitor() const
    {
        if(m_window)
            return VqMonitor(glfwGetWindowMonitor(m_window));
        return VqMonitor();
    }

    Vec2I       VqWindow::position() const
    {
        if(!m_window)
            return {};
        Vec2I   ret;
        glfwGetWindowPos(m_window, &ret.x, &ret.y);
        return ret;
    }

    void        VqWindow::restore()
    {
        if(m_window)
            glfwRestoreWindow(m_window);
    }

    void        VqWindow::set_position(const Vec2I& pos)
    {
        set_position(pos.x, pos.y);
    }
    
    void        VqWindow::set_position(int x, int y)
    {
        if(m_window){
            glfwSetWindowPos(m_window, x, y);
        }
    }

    void        VqWindow::set_size(const Size2I& sz)
    {
        set_size(sz.x, sz.y);
    }

    void        VqWindow::set_size(int w, int h)
    {
        if(m_window){
            glfwSetWindowSize(m_window, std::max(1, w), std::max(1, h));
        }
    }

    void        VqWindow::set_title(const char*z)
    {
        if(m_window && z){
            glfwSetWindowTitle(m_window, z);
        }
    }

    void        VqWindow::set_title(const std::string&z)
    {
        set_title(z.c_str());
    }

    bool        VqWindow::should_close() const
    {
        if(!m_window) 
            return true;
        return glfwWindowShouldClose(m_window);
    }

    void        VqWindow::show()
    {
        if(m_window)
            glfwShowWindow(m_window);
    }

    Size2I      VqWindow::size() const
    {
        if(!m_window)
            return {};
        Size2I  ret;
        glfwGetWindowSize(m_window, &ret.x, &ret.y);
        return ret;
    }

    int  VqWindow::width() const
    {
        if(!m_window)
            return 0;
    
        int ret;
        glfwGetWindowSize(m_window, &ret, nullptr);
        return ret;
    }
}
