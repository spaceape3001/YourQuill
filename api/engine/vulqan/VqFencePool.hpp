////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <vulkan/vulkan_core.h>
#include <vector>

namespace yq {
    class MainWindow;

    /*! \brief Fence pool
    
    */
    class VqFencePool : trait::not_copyable, trait::not_moveable {
        /*
            Inspired by 
            https://github.com/KhronosGroup/Vulkan-Samples/blob/master/framework/fence_pool.h
        */
    public:
    
        VqFencePool();
        VqFencePool(MainWindow&);
        VqFencePool(VkDevice);
        ~VqFencePool();

        bool        init(MainWindow&, bool reinit=false);
        bool        init(VkDevice, bool reinit=false);
        
        bool        initialized() const { return m_device != nullptr; }
        
        VkResult    wait(uint64_t timeout=UINT64_MAX, bool all=true) const;
        VkResult    wait_one(uint64_t timeout=UINT64_MAX) const;
        VkResult    wait_all(uint64_t timeout=UINT64_MAX) const;
        
        VkResult    reset(bool zero=true);
        VkFence     allocate();
        
        size_t      active() const { return m_active; }
        VkFence     fence(size_t) const;
        
        
    private:
        VkDevice                m_device = nullptr;
        std::vector<VkFence>    m_fences;
        size_t                  m_active    = 9;
        
        void                    dtor();
    
   };
}
