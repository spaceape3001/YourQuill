////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <vulkan/vulkan_core.h>
#include <set>
#include <vector>

struct GLFWwindow;


namespace yq {
    namespace engine {
        class VqSurface : trait::not_copyable {
        public:
            VqSurface() {}
            VqSurface(VkPhysicalDevice, GLFWwindow*);
            VqSurface(VqSurface&&);
            VqSurface&          operator=(VqSurface&&);
            ~VqSurface();
            
            VkSurfaceKHR        surface() const { return m_surface; }
            operator bool () const { return m_surface != nullptr; }
            operator VkSurfaceKHR() const { return m_surface; }
            
            VkFormat            format() const { return m_format; }
            VkColorSpaceKHR     color_space() const { return m_colorSpace; }
            
            VkSurfaceCapabilitiesKHR    capabilities() const;
            
            bool    supports(VkPresentModeKHR) const;
            bool    supports(VkFormat) const;
            
            VkColorSpaceKHR color_space(VkFormat) const;
            
        private:
            void    dtor();
            void    move(VqSurface&&);
            VkSurfaceKHR                        m_surface   = nullptr;
            VkPhysicalDevice                    m_physical  = nullptr;
            std::set<VkPresentModeKHR>          m_presentModes;
            std::vector<VkSurfaceFormatKHR>     m_formats;
            VkFormat                            m_format;
            VkColorSpaceKHR                     m_colorSpace;
        };
    
    }
}
