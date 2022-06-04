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
    namespace engine {

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
        
        /*! \brief Engine/Vulkan application
        
        */
        class VqApp : public BasicApp {
        public:
        
            //! Global application, if any
            static VqApp*       app() { return s_app; }
            
            /*! \brief Vulkan instance
            
                If necessary, this will initialize the vulkan *IFF* the app exists and vulkan's not already been
                initialized.
            */
            static VkInstance    instance();
        
            /*! \brief Constructor
            
                \param[in]  argc    Pass onto me what the main() was given
                \param[in]  argv    Pass onto me what the main() was given
                \param[in]  aci     Initialization paraemters for this application
            */
            VqApp(int argc, char* argv[], const AppCreateInfo& aci=AppCreateInfo());
            ~VqApp();
            
            
            //void    run();
            
            //!  exec loop tied to a single window
            void    run_window(VqWindow*, double timeout=0.);
            
            
            /*! \brief Initializes the GLFW
            
                This initializes the GLFW.  
                
                \note calling instance() or init_vulkan() will automatically initialize GLFW.
            */
            void    init_glfw();
            
            /*! \brief Initializes vulkan instance
            
                \note calling instance() will automatically call this, if necessary.
            */
            bool    init_vulkan();
            
        private:
            friend class VqWindow;
            
            static VqApp*               s_app;
            static VkInstance           s_instance;

            AppCreateInfo                       m_appInfo;
            VkInstance                          m_instance    = nullptr;
            std::vector<VkLayerProperties>      m_allLayerProps;
            std::vector<VkExtensionProperties>  m_allExtensionProps;
            std::set<std::string>               m_allLayerNames;
            std::set<std::string>               m_allExtensionNames;
            std::vector<const char*>            m_extensions;
            std::vector<const char*>            m_layers;
            VkDebugReportCallbackEXT            m_debug     = nullptr;
            bool                                m_glfw        = false;
            
            bool        init();
            void        kill();
            
        };

    }
}
