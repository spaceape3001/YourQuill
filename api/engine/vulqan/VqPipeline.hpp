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
        class Window;
        
        class VqPipeline : trait::not_copyable {
        public:
            VqPipeline(){}
            VqPipeline(Window*, const PipelineConfig&);
            VqPipeline(VqPipeline&&);
            VqPipeline& operator=(VqPipeline&&);
            ~VqPipeline();
            
            bool        good() const noexcept { return m_pipeline != nullptr; }
            VkPipeline  pipeline() const noexcept { return m_pipeline; }
            VkPipeline  wireframe() const noexcept { return m_wireframe; }

            operator VkPipeline() const noexcept { return m_pipeline; }
            
        private:
            void    dtor();
            void    move(VqPipeline&&);
        
            VkDevice            m_device    = nullptr;
            VkPipelineLayout    m_layout    = nullptr;
            VkPipeline          m_pipeline  = nullptr;
            VkPipeline          m_wireframe = nullptr;
            
        };

    }
}
