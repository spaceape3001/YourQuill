////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/BasicApp.hpp>
#include <basic/DbgRel.hpp>
#include <vulkan/vulkan_core.h>
#include <set>
#include <vector>

struct VkInstance_T;

namespace yq {

    enum class Required : uint8_t {
        NO,
        OPTIONAL,
        YES
    };


    //! Info for initialization
    struct AppCreateInfo {
    
        //! Application name
        std::string     app_name;
        
        //! Engine Name
        std::string     engine_name     = "(nameless)";
        
        //! Application version number
        uint32_t        app_version     = 0;
        
        //! Engine version number
        uint32_t        engine_version  = 0;
        
        //! Vulkan API version (zero will default to latest)
        uint32_t        vulkan_api      = 0;
        
        //! Add KHRONOS validation layer
        Required        validation      = YQ_DBGREL( Required::YES, Required::NO);
        
        
        AppCreateInfo(){}
    };


    class VqApp;
    class VqWindow;
    
    class VqApp : public BasicApp {
    public:
    
        static VqApp*       app() { return s_app; }
        static VkInstance_T*    instance();
    
        VqApp(int argc, char* argv[], const AppCreateInfo& ci=AppCreateInfo());
        ~VqApp();
        
        
        //void    run();
        
        //  exec loop tied to a single window
        void    run(VqWindow*);
        
    private:
        friend class VqWindow;
        
        static VqApp*               s_app;
        static VkInstance           s_instance;

        AppCreateInfo                       m_appInfo;
        VkInstance                          m_instance    = nullptr;
        bool                                m_glfw        = false;
        std::vector<VkLayerProperties>      m_allLayerProps;
        std::vector<VkExtensionProperties>  m_allExtensionProps;
        std::set<std::string>               m_allLayerNames;
        std::set<std::string>               m_allExtensionNames;
        std::vector<const char*>            m_extensions;
        std::vector<const char*>            m_layers;
        VkDebugReportCallbackEXT            m_debug     = nullptr;
        
        bool        init();
        void        kill();
        
    };

}
