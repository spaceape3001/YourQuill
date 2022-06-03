////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqApp.hpp"

#include <basic/Logging.hpp>
#include <basic/ThreadId.hpp>
#include <engine/vulqan/VqStructs.hpp>
#include <engine/vulqan/VqUtils.hpp>

#include <tbb/spin_mutex.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace yq {
    VqApp*          VqApp::s_app    = nullptr;
    VkInstance      VqApp::s_instance  = nullptr;

    static void    glfwLogging(int ec, const char* why)
    {
        static const auto  prior = glfwSetErrorCallback(glfwLogging);
        if(ec){
            if(!why)
                why = "Unknown error";
            yWarning() << "GLFW error (" << ec << "): " << why;
        }
        if(prior)
            prior(ec, why);
    }


    VkInstance_T*    VqApp::instance()
    {
        if(s_instance)  [[ likely ]]
            return s_instance;
        if(!s_app){
            yCritical() << "No Engine App has been instantiated!";
            return nullptr;
        }
        
        static tbb::spin_mutex  mutex;
        tbb::spin_mutex::scoped_lock    _m(mutex);
        if(s_instance)
            return s_instance;
        if(!s_app-> init())
            return nullptr;
        s_instance  = s_app -> m_instance;
        return s_instance;
    }

    //  ////////////////////////////////////////////////////////////////////////

    VqApp::VqApp(int argc, char *argv[], const AppCreateInfo& ci) : 
        BasicApp(argc, argv), m_appInfo(ci)
    {
        if(m_appInfo.app_name.empty())
           m_appInfo.app_name = app_name();
        if(m_appInfo.engine_name.empty())
            m_appInfo.engine_name = "YourQuill";
        if(!m_appInfo.vulkan_api)
            m_appInfo.vulkan_api  = VK_API_VERSION_1_2;

        if((!thread::id()) && !s_app)
            s_app   = this;
    }
    
    VqApp::~VqApp()
    {
        kill();
        if(this == s_app){
            s_app       = nullptr;
            s_instance  = nullptr;
        }
    }

        
    bool        VqApp::init()
    {
        glfwLogging(0,nullptr);
        glfwInit();
        m_glfw  = true;


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
        if(m_appInfo.validation != Required::NO){
            for(const char* z : kValidationLayers){
                if(m_allLayerNames.contains(z))
                    m_layers.push_back(z);
            }
            
            if(m_layers.size() != kValidationLayers.size()){
                auto stream    = (m_appInfo.validation == Required::YES) ? yCritical() : yError();
                stream << "Unable to find validation layers!";
                if(m_appInfo.validation == Required::YES)
                    return false;
            } else
                want_debug  = true;
        }

        m_extensions = vqGlfwRequiredExtensions();
        if(want_debug)
            m_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

            
            /*
                Create our device
            */
        
        VqApplicationInfo       lai;
        lai.pApplicationName    = m_appInfo.app_name.c_str();
        lai.applicationVersion  = m_appInfo.app_version;
        lai.pEngineName         = m_appInfo.engine_name.c_str();
        lai.engineVersion       = m_appInfo.engine_version;
        lai.apiVersion          = m_appInfo.vulkan_api;
        
        VqInstanceCreateInfo    createInfo;
        createInfo.pApplicationInfo             = &lai;
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
        
        
        if(m_instance == nullptr)
            yCritical() << "Vulkan instance is NULL!";
        else
            yInfo() << "Vulkan instance created.";

        return true;
    }
    
    void        VqApp::kill()
    {
        if(m_instance){
            vkDestroyInstance(m_instance, nullptr);
            m_instance  = nullptr;
        }
        
        if(m_glfw){
            glfwTerminate();
            m_glfw  = false;
        }
    }

}
