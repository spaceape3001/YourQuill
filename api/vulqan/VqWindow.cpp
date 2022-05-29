////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqWindow.hpp"
#include "VqCore.hpp"
#include "VqInstance.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <basic/Logging.hpp>

#include <cassert>

namespace yq {
    void VqWindow::poll_events()
    {
        glfwPollEvents();
    }
    
    ////////////////////////////////////////////////////////////////////////////////

    VqWindow::VqWindow(Ref<VqInstance> vi) : m_vulkan(vi)
    {
        assert(vi.valid() && "Needs a valid vulkan instance!");
    }
    
    VqWindow::VqWindow(Ref<VqInstance> vi, const Info&i) : VqWindow(vi)
    {
        initialize(i);
    }
    
    VqWindow::~VqWindow()
    {
        close();
    }

    void VqWindow::_deinit()
    {
        if(m_logicalDevice){
            vkDestroyDevice(m_logicalDevice, nullptr);
            m_logicalDevice   = nullptr;
        }

        if(m_surface){
            vkDestroySurfaceKHR(m_vulkan->m_instance, m_surface, nullptr);
            m_surface  = nullptr;
        }

        if(m_window){
            glfwDestroyWindow(m_window);
            m_window    = nullptr;
        }
    }

    void VqWindow::close()
    {
        if(m_init){
            _deinit();
            m_init  = false;
        }
    }

    bool VqWindow::initialize(const Info& i)
    {
        if(m_init)
            return true;
        
        if(!m_vulkan->good()){
            yError() << "Cannot create window with uninitialized vulkan!";
            return false;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        
        GLFWmonitor*    mon = nullptr;
        if(i.is_full)
            mon     = glfwGetPrimaryMonitor();

        m_window = glfwCreateWindow(std::max(1,i.size.width()), std::max(1,i.size.height()), i.title, mon, nullptr);
        if(!m_window){
            yError() << "Unable to create window.";
            return false;
        }
        
        glfwSetWindowUserPointer(m_window, this);
        
            /*
                Create our surface
            */
        
        if(glfwCreateWindowSurface(m_vulkan->m_instance, m_window, nullptr, &m_surface) != VK_SUCCESS){
            yCritical() << "Unable to create window surface!";
            return false;
        }

        /*
            Create our logical device
        */
    
        auto queueInfos         = vqGetPhysicalDeviceQueueFamilyProperties(m_vulkan->m_physDevice);
        Result<uint32_t> gqu    = vqFindFirstGraphicsQueue(queueInfos);
        if(!gqu.good){
            yCritical() << "Unable to get a queue with graphic capability!";
            _deinit();
            return false;
        }
    
        Result<uint32_t>    pqu;
        for(uint32_t i=0;i<(uint32_t) queueInfos.size();++i){
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(m_vulkan->m_physDevice, i, m_surface, &presentSupport);
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
        
        dci.enabledLayerCount        = (uint32_t) m_vulkan->m_layers.size();
        if(dci.enabledLayerCount)
            dci.ppEnabledLayerNames  = m_vulkan->m_layers.data();
    
        dci.enabledExtensionCount    = 0;
        
        if(vkCreateDevice(m_vulkan->m_physDevice, &dci, nullptr, &m_logicalDevice) != VK_SUCCESS){
            yCritical() << "Unable to create logical device!";
            _deinit();
            return false;
        }
        
            /*
                Graphics queue
            */
        vkGetDeviceQueue(m_logicalDevice, gqu.value, 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_logicalDevice, pqu.value, 0, &m_presentQueue);
    
        m_init  = true;
        return true;
    }

    bool        VqWindow::should_close() const
    {
        return glfwWindowShouldClose(m_window);
    }

}
