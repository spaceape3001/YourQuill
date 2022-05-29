////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqInstance.hpp"
#include "VqCore.hpp"
#include "VqGLFW.hpp"

#include <basic/CmdArgs.hpp>
#include <basic/CollectionUtils.hpp>
#include <basic/Logging.hpp>

namespace yq {
    VqInstance::VqInstance()
    {
    }
    
    VqInstance::VqInstance(const Info& i) : VqInstance()
    {
        initialize(i);
    }

    VqInstance::~VqInstance()
    {
        shutdown();
    }
    
    void  VqInstance::_deinit()
    {
        m_physDevice    = nullptr;
        if(m_instance){
            vkDestroyInstance(m_instance, nullptr);
            m_instance    = nullptr;
        }
        m_layers.clear();
        if(m_glfw){
            glfwTerminate();
            m_glfw      = false;
        }
    }

    bool  VqInstance::initialize(const Info& i)
    {
        if(m_init)
            return true;
            
        m_glfw  = i.glfw_enable && i.glfw_init;
        if(m_glfw){
            glfwSetErrorCallback([](int ec, const char* why){
                if(!why)
                    why = "Unknown error";
                yWarning() << "GLFW error (" << ec << "): " << why;
            });
            glfwInit();
        }
        
        static const std::vector<const char*>   kValidationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        bool    want_debug  = false;

        /*
            Start by scanning the extensions for validation
        */
        if(i.validation != Required::NO){
            std::set<std::string>     available   = vqNameSet(vqEnumerateInstanceLayerProperties());
            for(const char* z : kValidationLayers){
                if(available.contains(z))
                    m_layers.push_back(z);
            }
            
            if(m_layers.size() != kValidationLayers.size()){
                auto stream    = (i.validation == Required::YES) ? yCritical() : yError();
                stream << "Unable to find validation layers!";
                if(i.validation == Required::YES){
                    _deinit();
                    return false;
                }
            } else
                want_debug  = true;
        }

        std::vector<const char*>     extensions;
        if(i.glfw_enable){
            extensions = vqGlfwRequiredExtensions();
        }
        if(want_debug)
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            
            /*
                Create our device
            */
        
        VqApplicationInfo       appInfo;
        appInfo.pApplicationName                = i.app_name;
        appInfo.applicationVersion              = i.app_version;
        appInfo.pEngineName                     = i.engine_name;
        appInfo.engineVersion                   = i.engine_version;
        appInfo.apiVersion                      = i.vulkan_api;
        
        VqInstanceCreateInfo    createInfo;
        createInfo.pApplicationInfo             = &appInfo;
        createInfo.enabledLayerCount            = (uint32_t) m_layers.size();
        if(createInfo.enabledLayerCount)
            createInfo.ppEnabledLayerNames      = m_layers.data();
            
        createInfo.enabledExtensionCount        = (uint32_t) extensions.size();
        if(!extensions.empty())
            createInfo.ppEnabledExtensionNames  = extensions.data();
            
        yInfo() << "Enabled Layer Count -> " << createInfo.enabledLayerCount;
        
        if(vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS){
            yCritical() << "Unable to create vulkan instance!";
            m_instance   = nullptr;
            _deinit();
            return false;
        }
        
        
            /*
                Get the physical device
                We'll make it smarter (later)
            */
        auto devices = vqEnumeratePhysicalDevices(m_instance);
        yInfo() << "Found " << devices.size() << " devices";
        for(VkPhysicalDevice dev : devices){
            if(!m_physDevice)
                m_physDevice   = dev;  // grab the first one
            VkPhysicalDeviceProperties  props;
            vkGetPhysicalDeviceProperties(dev, &props);
            yInfo() << "Device: " << props.deviceName;
            break;
        }
        if(!m_physDevice){
            yCritical() << "Unable to find a physical device!";
            _deinit();
            return false;
        }
     
        m_init  = true;
        return true;
    }
    
    void  VqInstance::shutdown()
    {
        if(!m_init)
            return ;
        _deinit();
        m_init  = false;
    }
}
