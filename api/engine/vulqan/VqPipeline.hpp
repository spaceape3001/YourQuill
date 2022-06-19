////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <vulkan/vulkan_core.h>

namespace yq {
    namespace engine {
        struct PipelineConfig;
        struct VqInternal;
        
        class VqPipeline : trait::not_copyable {
        public:
            VqPipeline(){}
            VqPipeline(VqInternal&, const PipelineConfig&);
            VqPipeline(VqPipeline&&);
            VqPipeline& operator=(VqPipeline&&);
            ~VqPipeline();
            
            bool                good() const noexcept { return m_pipeline != nullptr; }
            VkPipeline          pipeline() const noexcept { return m_pipeline; }
            VkPipeline          wireframe() const noexcept { return m_wireframe; }
            VkPipelineLayout    layout() const noexcept { return m_layout; }

            operator VkPipeline() const noexcept { return m_pipeline; }
            
            uint32_t             shader_mask() const noexcept { return m_shaderMask; }
            
        private:
            void    dtor();
            void    move(VqPipeline&&);
        
            VkDevice            m_device    = nullptr;
            VkPipelineLayout    m_layout    = nullptr;
            VkPipeline          m_pipeline  = nullptr;
            VkPipeline          m_wireframe = nullptr;
            uint32_t            m_shaderMask    = 0;
        };

    }
}
