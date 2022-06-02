////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "EngineApp.hpp"
#include "EngineAppImpl.hpp"

#include <basic/Logging.hpp>
#include <basic/ThreadId.hpp>
#include <engine/vulqan/VqStructs.hpp>
#include <engine/vulqan/VqUtils.hpp>

#include <tbb/spin_mutex.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace yq {
    EngineApp*    EngineApp::s_app    = nullptr;
    VkInstance_T* EngineApp::s_instance  = nullptr;

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


    VkInstance_T*    EngineApp::instance()
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
        if(!s_app-> m->init())
            return nullptr;
        s_instance  = s_app -> m -> instance;
        return s_instance;
    }

    //  ////////////////////////////////////////////////////////////////////////

    EngineApp::EngineApp(int argc, char *argv[], const EngineCreateInfo& ci) : 
        BasicApp(argc, argv), m(new Impl(ci))
    {
        m   = std::make_unique<Impl>(ci);
        if((!thread::id()) && !s_app)
            s_app   = this;
        
    }
    
    EngineApp::~EngineApp()
    {
        if(this == s_app){
            s_app       = nullptr;
            s_instance  = nullptr;
        }
    }

    //  ////////////////////////////////////////////////////////////////////////

    EngineApp::Impl::Impl(const EngineCreateInfo& ci)
    {
        appInfo = ci;
        if(appInfo.app_name.empty())
           appInfo.app_name = app_name();
        if(appInfo.engine_name.empty())
            appInfo.engine_name = "YourQuill";
        if(!appInfo.vulkan_api)
            appInfo.vulkan_api  = VK_API_VERSION_1_2;
    }

    EngineApp::Impl::~Impl()
    {
        kill();
    }
        
    bool        EngineApp::Impl::init()
    {
        glfwLogging(0,nullptr);
        glfwInit();
        glfw  = true;


        static const std::vector<const char*>   kValidationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
        
        allLayerProps     = vqEnumerateInstanceLayerProperties();
        allLayerNames     = vqNameSet(allLayerProps);
        
        allExtensionProps = vqEnumerateInstanceExtensionProperties();
        allExtensionNames = vqNameSet(allExtensionProps);

        bool    want_debug  = false;

        /*
            Start by scanning the extensions for validation
        */
        if(appInfo.validation != Required::No){
            for(const char* z : kValidationLayers){
                if(allLayerNames.contains(z))
                    layers.push_back(z);
            }
            
            if(layers.size() != kValidationLayers.size()){
                auto stream    = (appInfo.validation == Required::Yes) ? yCritical() : yError();
                stream << "Unable to find validation layers!";
                if(appInfo.validation == Required::Yes)
                    return false;
            } else
                want_debug  = true;
        }

        extensions = vqGlfwRequiredExtensions();
        if(want_debug)
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

            
            /*
                Create our device
            */
        
        VqApplicationInfo       lai;
        lai.pApplicationName    = appInfo.app_name.c_str();
        lai.applicationVersion  = appInfo.app_version;
        lai.pEngineName         = appInfo.engine_name.c_str();
        lai.engineVersion       = appInfo.engine_version;
        lai.apiVersion          = appInfo.vulkan_api;
        
        VqInstanceCreateInfo    createInfo;
        createInfo.pApplicationInfo             = &lai;
        createInfo.enabledLayerCount            = (uint32_t) layers.size();
        if(createInfo.enabledLayerCount)
            createInfo.ppEnabledLayerNames      = layers.data();
            
        createInfo.enabledExtensionCount        = (uint32_t) extensions.size();
        if(createInfo.enabledExtensionCount)
            createInfo.ppEnabledExtensionNames  = extensions.data();
            
        yInfo() << "Enabled Layer Count -> " << createInfo.enabledLayerCount;
        
        if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS){
            yCritical() << "Unable to create vulkan instance!";
            instance   = nullptr;
            return false;
        }
        
        
        if(instance == nullptr)
            yCritical() << "Vulkan instance is NULL!";
        else
            yInfo() << "Vulkan instance created.";

        return true;
    }
    
    void        EngineApp::Impl::kill()
    {
        if(instance){
            vkDestroyInstance(instance, nullptr);
            instance  = nullptr;
        }
        
        if(glfw){
            glfwTerminate();
            glfw  = false;
        }
    }





    


}
