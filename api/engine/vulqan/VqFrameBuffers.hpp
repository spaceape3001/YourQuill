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
    class VqRenderPass;

    class VqFrameBuffers : trait::not_copyable {
    public:
        VqFrameBuffers() {}
        VqFrameBuffers(VkDevice, VqRenderPass&, const VkExtent2D&, const std::vector<VkImageView>&images);
        VqFrameBuffers(VqFrameBuffers&&);
        VqFrameBuffers&          operator=(VqFrameBuffers&&);
        ~VqFrameBuffers();
        
        VkFramebuffer           operator[](size_t) const;
        VkFramebuffer           framebuffer(size_t) const;
        bool                    empty() const { return m_framebuffers.empty(); }
        size_t                  size() const { return m_framebuffers.size(); }
        size_t                  count() const { return m_framebuffers.size(); }
        
    private:
        
        void    dtor();
        void    move(VqFrameBuffers&&);

        VkDevice                    m_device;
        std::vector<VkFramebuffer>  m_framebuffers;
    };

}
