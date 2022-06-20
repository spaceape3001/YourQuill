////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VqException.hpp"
#include "VqDevice.hpp"
#include "VqStructs.hpp"
#include "VqSurface.hpp"
#include "VqUtils.hpp"

#include <basic/preamble.hpp>
#include <engine/Application.hpp>

namespace yq {
    VqDevice::VqDevice(const VqSurface&surf, const Config& dci)
    {
        const Application*   app    = Application::app();
        if(!app)
            throw VqException("Unintialized or no application present!");

        
        auto queueInfos         = vqFindQueueFamilies(surf.physical(), surf.surface());
        if(!queueInfos.graphics.has_value())
            throw VqException("No graphic queue capability!");
        if(!queueInfos.present.has_value())
            throw VqException("No present queue capability!");
        
        std::vector<VkDeviceQueueCreateInfo> qci;

        uint32_t    graphicFamily   = queueInfos.graphics.value();
        uint32_t    presentFamily   = queueInfos.present.value();
        
        uint32_t    graphicCount    = 1;
        uint32_t    presentCount    = 1;
        
        std::vector<float> graphicWeights;
        std::vector<float> presentWeights;
        
        if(const uint32_t* p = std::get_if<uint32_t>(&dci.graphics)){
            if(*p != 0){
                graphicCount    = *p;
                graphicWeights.resize(graphicCount, 1.f);
            }
        }
        if(const std::vector<float>*p = std::get_if<std::vector<float>>(&dci.graphics)){
            if(!p->empty()){
                graphicWeights  = *p;
                graphicCount    = p->size();
            }
        }
        
        if(graphicWeights.size() < graphicCount)
            graphicWeights.resize(graphicCount, 1.f);
        
        if(const uint32_t* p = std::get_if<uint32_t>(&dci.present)){
            if(*p != 0){
                presentCount    = *p;
                presentWeights.resize(presentCount, 1.f);
            }
        }
        if(const std::vector<float>*p = std::get_if<std::vector<float>>(&dci.present)){
            if(!p->empty()){
                presentWeights  = *p;
                presentCount   = p->size();
            }
        }
        if(presentWeights.size() < presentCount)
            presentWeights.resize(presentCount, 1.f);
        
            
        if(graphicFamily != presentFamily){
            //  separate family....
            
            VqDeviceQueueCreateInfo info;
            info.queueFamilyIndex   = graphicFamily;
            info.queueCount         = graphicCount;
            info.pQueuePriorities   = graphicWeights.data();
            qci.push_back(info);
            
            info.queueFamilyIndex   = presentFamily;
            info.queueCount         = presentCount;
            info.pQueuePriorities   = presentWeights.data();
            qci.push_back(info);
        } else {
            const std::vector<float>&  weights = (presentWeights.size() < graphicWeights.size()) ? graphicWeights : presentWeights;
            VqDeviceQueueCreateInfo info;
            info.queueFamilyIndex   = graphicFamily;
            info.queueCount         = std::max(graphicCount, presentCount);
            info.pQueuePriorities   = weights.data();
            qci.push_back(info);
        }
            

        VkPhysicalDeviceFeatures    df{};
        df.fillModeNonSolid         = VK_TRUE;

        VqDeviceCreateInfo          createInfo;
        createInfo.pQueueCreateInfos        = qci.data();
        createInfo.queueCreateInfoCount     = (uint32_t) qci.size();
        createInfo.pEnabledFeatures         = &df;
        
        const auto& layers = app->layers();
        
        createInfo.enabledLayerCount        = (uint32_t) layers.size();
        if(createInfo.enabledLayerCount)
            createInfo.ppEnabledLayerNames  = layers.data();
    
        createInfo.enabledExtensionCount    = (uint32_t) dci.extensions.size();
        if(!dci.extensions.empty())
            createInfo.ppEnabledExtensionNames = dci.extensions.data();
        
        if(vkCreateDevice(surf.physical(), &createInfo, nullptr, &m_device) != VK_SUCCESS)
            throw VqException("Unable to create logical device!");
        
            /*
                Graphics queues
            */
        m_graphics  = VqQueues(m_device, graphicFamily, graphicCount );
        m_present   = VqQueues(m_device, presentFamily, presentCount );
    }

    void        VqDevice::dtor()
    {
        m_graphics  = {};
        m_present   = {};
        if(m_device){
            vkDestroyDevice(m_device, nullptr);
            m_device    = nullptr;
        }
    }
    
    VqDevice::VqDevice(VqDevice&&mv)
    {
        move(std::move(mv));
    }
    
    VqDevice&    VqDevice::operator=(VqDevice&& mv)
    {
        if(this != &mv){
            dtor();
            move(std::move(mv));
        }
        return *this;
    }
    
    VqDevice::~VqDevice()
    {
        dtor();
    }
    
    void    VqDevice::move(VqDevice&&mv)
    {
        steal(m_device, mv.m_device);
        m_graphics  = std::move(mv.m_graphics);
        m_present   = std::move(mv.m_present);
    }

    VkQueue     VqDevice::graphics(uint32_t i) const
    {
        return m_graphics[i];
    }
    
    VkQueue     VqDevice::present(uint32_t i) const
    {
        return m_present[i];
    }
}
