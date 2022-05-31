////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqApp.hpp"
#include "VqUtils.hpp"
#include <basic/Logging.hpp>
#include <basic/PluginLoader.hpp>
#include <basic/ThreadId.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace yq {
    VqApp*     VqApp::s_app     = nullptr;
    VkInstance VqApp::s_instance = nullptr;

    typedef void (*FNErrorCallback)(int, const char*);

    static void    glfwLogging(int ec, const char* why)
    {
        static const FNErrorCallback  prior = glfwSetErrorCallback(glfwLogging);
        if(ec){
            if(!why)
                why = "Unknown error";
            yWarning() << "GLFW error (" << ec << "): " << why;
        }
        if(prior)
            prior(ec, why);
    }

    VkInstance       VqApp::instance()
    {
        if(s_instance) [[likely]]
            return s_instance;
            
        if(s_app){
            if(!s_app->m_instance) {
                if(!s_app -> init()){
                    yFatal() << "Unable to initialize the application!";
                } else 
                    s_instance  = s_app -> m_instance;
            }
            return s_app->m_instance;
        }
        return nullptr;
    }

    VqApp::VqApp(int argc, char* argv[], const AppInfo& ai) : BasicApp(argc, argv), m_appInfo(ai)
    {
        if(m_appInfo.app_name.empty())
           m_appInfo.app_name    = app_name(); 

        if(!s_app && !thread::id()){
            s_app   = this;
            glfwLogging(0, nullptr);
        }
    }
    
    VqApp::~VqApp()
    {
        if(s_app == this){
            s_instance  = nullptr;
            s_app       = nullptr;
        }
    }

    void    VqApp::deinit()
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
    
    bool    VqApp::init()
    {
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
        
        VqApplicationInfo       appInfo;
        appInfo.pApplicationName                = m_appInfo.app_name.c_str();
        appInfo.applicationVersion              = m_appInfo.app_version;
        appInfo.pEngineName                     = m_appInfo.engine_name.c_str();
        appInfo.engineVersion                   = m_appInfo.engine_version;
        appInfo.apiVersion                      = m_appInfo.vulkan_api;
        
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

        return true;
    }
}
