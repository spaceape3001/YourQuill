////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VqException.hpp"
#include "VqSurface.hpp"
#include "VqUtils.hpp"

#include <basic/CollectionUtils.hpp>
#include <engine/Application.hpp>

#include <GLFW/glfw3.h>

namespace yq {
    namespace engine {
        VqSurface::VqSurface(VkPhysicalDevice phys, GLFWwindow* win) : m_physical(phys)
        {
            if(win){
                if(glfwCreateWindowSurface(Application::vulkan(), win, nullptr, &m_surface) != VK_SUCCESS)
                    throw VqException("Unable to create window surface!");
                m_presentModes  = make_set(vqGetPhysicalDeviceSurfacePresentModesKHR(phys, m_surface));
                m_formats       = vqGetPhysicalDeviceSurfaceFormatsKHR(phys, m_surface);
                
                // right now, cheating
                m_format        = VK_FORMAT_B8G8R8A8_SRGB;
                m_colorSpace    = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
            }
        }
        
        VqSurface::VqSurface(VqSurface&&mv)
        {
            move(std::move(mv));
        }
        
        VqSurface&          VqSurface::operator=(VqSurface&&mv)
        {
            if(this != &mv){
                dtor();
                move(std::move(mv));
            }
            return *this;
        }
        
        VqSurface::~VqSurface()
        {
            dtor();
        }

        VkSurfaceCapabilitiesKHR    VqSurface::capabilities() const
        {
            VkSurfaceCapabilitiesKHR    ret;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical, m_surface, &ret);
            return ret;
        }

        VkColorSpaceKHR VqSurface::color_space(VkFormat fmt) const
        {
            for(auto& f : m_formats)
                if(fmt == f.format)
                    return f.colorSpace;
            return VK_COLOR_SPACE_MAX_ENUM_KHR;
        }

        void VqSurface::dtor()
        {
            if(m_surface){
                vkDestroySurfaceKHR(Application::vulkan(), m_surface, nullptr);
                m_surface   = nullptr;
                m_physical  = nullptr;
            }
        }

        void    VqSurface::move(VqSurface&&mv)
        {
            steal(m_surface, mv.m_surface);
            steal(m_physical, mv.m_physical);
            m_presentModes  = std::move(mv.m_presentModes);
            m_formats       = std::move(mv.m_formats);
            m_format        = mv.m_format;
            m_colorSpace    = mv.m_colorSpace;
        }

        bool    VqSurface::supports(VkPresentModeKHR p) const
        {
            return m_presentModes.contains(p);
        }
        
        bool    VqSurface::supports(VkFormat fmt) const
        {
            for(auto& f : m_formats)
                if(fmt == f.format)
                    return true;
            return false;
        }
    }
}

