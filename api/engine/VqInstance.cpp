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
    VqInstance*     VqInstance::s_singleton = nullptr;
    VkInstance      VqInstance::s_vulkan = nullptr;

    const VqInstance*    VqInstance::singleton()
    {
        return s_singleton;
    }
    
    bool                 VqInstance::initialized()
    {
        return s_singleton != nullptr;
    }

    VkInstance           VqInstance::vulkan()
    {
        return s_vulkan;
    }
    

    
    VqInstance::VqInstance(const Info& i)
    {
        if((!s_singleton) && init(i)){
            s_vulkan  = m_instance;
            s_singleton = this;
        }
    }

    VqInstance::~VqInstance()
    {
        if(this == s_singleton){
            s_vulkan  = nullptr;
            vkDestroyInstance(m_instance, nullptr);
            m_instance  = nullptr;
            s_singleton = nullptr;
        }
    }
    
    bool  VqInstance::init(const Info& i)
    {
        
        bool glfw = VqGLFW::initialized();
            
        static const std::vector<const char*>   kValidationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
        
        m_allLayerProps     = vqEnumerateInstanceLayerProperties();
        m_allLayerNames     = vqNameSet(m_allLayerProps);
        
        m_allExtensionProps = vqEnumerateInstanceExtensionProperties();
        m_allExtensionNames = vqNameSet(m_allExtensionProps);

        bool    want_debug  = false;

        /*
            Start by scanning the extensions for validation
        */
        if(i.validation != Required::NO){
            for(const char* z : kValidationLayers){
                if(m_allLayerNames.contains(z))
                    m_layers.push_back(z);
            }
            
            if(m_layers.size() != kValidationLayers.size()){
                auto stream    = (i.validation == Required::YES) ? yCritical() : yError();
                stream << "Unable to find validation layers!";
                if(i.validation == Required::YES)
                    return false;
            } else
                want_debug  = true;
        }

        if(glfw){
            m_extensions = vqGlfwRequiredExtensions();
        }
        if(want_debug)
            m_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            
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
            
        createInfo.enabledExtensionCount        = (uint32_t) m_extensions.size();
        if(createInfo.enabledExtensionCount)
            createInfo.ppEnabledExtensionNames  = m_extensions.data();
            
        yInfo() << "Enabled Layer Count -> " << createInfo.enabledLayerCount;
        
        if(vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS){
            yCritical() << "Unable to create vulkan instance!";
            m_instance   = nullptr;
            return false;
        }
        
        #if 0
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
        #endif
     
        return true;
    }
    
}
