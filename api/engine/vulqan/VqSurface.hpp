////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <vulkan/vulkan_core.h>


struct GLFWwindow;


namespace yq {
    namespace engine {
        class VqSurface : trait::not_copyable {
        public:
            VqSurface() {}
            VqSurface(GLFWwindow*);
            VqSurface(VqSurface&&);
            ~VqSurface();
            VkSurfaceKHR        surface() const { return m_surface; }
            VqSurface&          operator=(VqSurface&&);
            
            operator bool () const { return m_surface != nullptr; }
            operator VkSurfaceKHR() const { return m_surface; }
            
        private:
            void                    dtor();
            VkSurfaceKHR           m_surface   = nullptr;
        };
    
    }
}
