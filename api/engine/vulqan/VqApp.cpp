////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqApp.hpp"
#include "VqWindow.hpp"

#include <basic/Logging.hpp>
#include <basic/ThreadId.hpp>
#include <engine/vulqan/VqStructs.hpp>
#include <engine/vulqan/VqUtils.hpp>

#include <tbb/spin_mutex.h>

#include <GLFW/glfw3.h>

namespace yq {
    namespace engine {

        static constexpr const uint32_t kEngineVersion      = YQ_MAKE_VERSION(0, 0, 1);
        static constexpr const char*    szEngineName        = "YourQuill";
    
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

        
        VkBool32 vqDebuggingCallback(
            VkDebugReportFlagsEXT                       flags,
            VkDebugReportObjectTypeEXT                  objectType,
            uint64_t                                    object,
            size_t                                      location,
            int32_t                                     messageCode,
            const char*                                 pLayerPrefix,
            const char*                                 pMessage,
            void*                                       pUserData
        )
        {
            log4cpp::CategoryStream         yell    = (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) ? yError("vulqan") : ((flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) ? yWarning("vulqan") : yInfo("vulqan"));
            yell << "Object [" << to_string(objectType) << ": " << object << "] (layer " << pLayerPrefix << "): " << pMessage;
            return VK_FALSE;
        }

        std::string_view    VqApp::engine_name() 
        {
            return szEngineName;
        }

        uint32_t            VqApp::engine_version() 
        {
            return kEngineVersion;
        }

        VkInstance_T*       VqApp::instance()
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

        bool    VqApp::add_layer(const char*z)
        {
            if(!z)
                return false;
            
            static const auto availableLayers     = vqNameSet(vqEnumerateInstanceLayerProperties());
            if(availableLayers.contains(z)){
                m_layers.push_back(z);
                return true;
            }
            return false;
        }
        
        bool    VqApp::add_extension(const char*z)
        {
            if(!z)
                return false;

            static const auto availableExtensions = vqNameSet(vqEnumerateInstanceExtensionProperties());
            if(availableExtensions.contains(z)){
                m_extensions.push_back(z);
                return true;
            }
            return false;
        }

        bool    VqApp::init()
        { 
            return init_vulkan();
        }
            
        void    VqApp::init_glfw()
        {
            if(!m_glfw){
                glfwLogging(0,nullptr);
                glfwInit();
                m_glfw  = true;
            }
        }

        bool        VqApp::init_vulkan()
        {
            init_glfw();
            if(m_instance)
                return true;

            static const char*  kValidationLayer        = "VK_LAYER_KHRONOS_validation";
            
            bool    want_debug  = false;

            /*
                Start by scanning the extensions for validation
            */
            if(m_appInfo.validation != Required::NO){
                if(!add_layer(kValidationLayer)){
                    auto stream    = (m_appInfo.validation == Required::YES) ? yCritical() : yError();
                    stream << "Unable to find validation layers!";
                    if(m_appInfo.validation == Required::YES)
                        return false;
                } else
                    want_debug  = true;
            }

            m_extensions = vqGlfwRequiredExtensions();
            if(want_debug){
                m_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
                m_extensions.push_back("VK_EXT_debug_report");
            }
            
            for(auto& x : m_appInfo.layers){
                if(!x.name)
                    continue;
                if(add_layer(x.name)){
                    yInfo() << "Enabling vulkan layer: " << x.name;
                    continue;
                }
                if(x.req != Required::NO){
                    auto stream    = (x.req == Required::YES) ? yCritical() : yError();
                    stream << "Unable to find vulkan layer: " << x.name;
                    if(x.req == Required::YES)
                        return false;
                }
            }

            for(auto& x : m_appInfo.extensions){
                if(!x.name)
                    continue;
                if(add_extension(x.name)){
                    yInfo() << "Enabling vulkan extension: " << x.name;
                    continue;
                }
                if(x.req != Required::NO){
                    auto stream    = (x.req == Required::YES) ? yCritical() : yError();
                    stream << "Unable to find vulkan extension: " << x.name;
                    if(x.req == Required::YES)
                        return false;
                }
            }
                
                /*
                    Create our device
                */
            
            VqApplicationInfo       lai;
            lai.pApplicationName    = m_appInfo.app_name.c_str();
            lai.applicationVersion  = m_appInfo.app_version;
            lai.pEngineName         = szEngineName;
            lai.engineVersion       = kEngineVersion;
            lai.apiVersion          = m_appInfo.vulkan_api;
            
            VqInstanceCreateInfo    createInfo;
            createInfo.pApplicationInfo             = &lai;
            createInfo.enabledLayerCount            = (uint32_t) m_layers.size();
            if(createInfo.enabledLayerCount)
                createInfo.ppEnabledLayerNames      = m_layers.data();
                
            createInfo.enabledExtensionCount        = (uint32_t) m_extensions.size();
            if(createInfo.enabledExtensionCount)
                createInfo.ppEnabledExtensionNames  = m_extensions.data();
                
            if(vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS){
                yCritical() << "Unable to create vulkan instance!";
                m_instance   = nullptr;
                return false;
            }
            
            if(m_instance == nullptr){
                yCritical() << "Vulkan instance is NULL!";
                return false;
            } else
                yInfo() << "Vulkan instance created.";
                
            if(want_debug){
                // Get the function pointer (required for any extensions)
                auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugReportCallbackEXT");
                VqDebugReportCallbackCreateInfoEXT debug_report_ci;
                debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
                debug_report_ci.pfnCallback = vqDebuggingCallback;
                debug_report_ci.pUserData = nullptr;
                vkCreateDebugReportCallbackEXT(m_instance, &debug_report_ci, nullptr, &m_debug);
            }

            return true;
        }
        
        void        VqApp::kill()
        {
            if(m_debug){
                auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT");
                vkDestroyDebugReportCallbackEXT(m_instance, m_debug, nullptr);
            }
        
            if(m_instance){
                vkDestroyInstance(m_instance, nullptr);
                m_instance  = nullptr;
            }
            
            if(m_glfw){
                glfwTerminate();
                m_glfw  = false;
            }
        }

        void    VqApp::run_window(VqWindow* win, double amt)
        {
            if(!win)
                return;
            while(!win->should_close()){
                if(amt<=0.){
                    glfwPollEvents();
                } else { 
                    glfwWaitEventsTimeout(amt);
                }
                win->draw();
            }
            
            vkDeviceWaitIdle(win->device());
        }

    }
}
