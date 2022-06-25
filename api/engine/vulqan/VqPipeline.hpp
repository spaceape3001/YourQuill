////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <vulkan/vulkan_core.h>
#include <engine/enum/DataActivity.hpp>
#include <engine/enum/PushConfigType.hpp>

namespace yq {
    namespace engine {
        struct PipelineConfig;
        struct VqInternal;
        class Pipeline;


        struct VqVBO {  // temporarily here....
            DataActivity    freq;
        };
        
        class VqPipeline : trait::not_copyable, trait::not_moveable {
        public:
        
            VqPipeline(VqInternal&, const PipelineConfig&);
            VqPipeline(VqInternal&, const Pipeline*);
            ~VqPipeline();
            
            bool                    good() const noexcept { return m_pipeline != nullptr; }
            VkPipeline              pipeline() const noexcept { return m_pipeline; }
            VkPipeline              wireframe() const noexcept { return m_wireframe; }
            VkPipelineLayout        layout() const noexcept { return m_layout; }
            
            uint32_t                shader_mask() const noexcept { return m_shaderMask; }
            
            uint64_t                id() const { return m_id; }
            
        private:
            void    dtor();
            
            //  All defined VBOS, in binding order
            std::vector<VqVBO>      m_vbos;
            VkDevice                m_device    = nullptr;
            VkPipelineLayout        m_layout    = nullptr;
            VkPipeline              m_pipeline  = nullptr;
            VkPipeline              m_wireframe = nullptr;
            uint64_t                m_id            = 0;
            uint32_t                m_shaderMask    = 0;
            PushConfigType          m_push      = PushConfigType::None;
        };

    }
}
