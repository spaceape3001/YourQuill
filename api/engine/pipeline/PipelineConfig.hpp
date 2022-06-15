////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/shader/ShaderPointer.hpp>
#include <optional>
#include <vector>
#include <engine/enum/CullMode.hpp>
#include <engine/enum/PolygonMode.hpp>
#include <engine/enum/Topology.hpp>
#include <engine/enum/FrontFace.hpp>
#include <engine/enum/DataFormat.hpp>
#include <engine/enum/PushConstant.hpp>
#include <engine/enum/VertexInputRate.hpp>

namespace yq {
    namespace engine {
    
        struct VBOAttr {
            DataFormat      format;
            uint32_t        location    = UINT32_MAX;
            uint32_t        offset      = 0;
        };
    
        struct VBOConfig {
            std::vector<VBOAttr>    attrs;
            uint32_t                stride      = 0;
            VertexInputRate         inputRate;
        };
        
        struct PushConfig {
            enum Type {
                None        = 0,
                Full,
                View,
                Custom
            };
            
            size_t          size    = 0;
            Type            type    = None;
        };
        
        struct PipelineConfig {
            std::vector<ShaderSpec>     shaders;
            std::vector<VBOConfig>      vbos;
            Topology                    topology    = Topology::TriangleList;
            PolygonMode                 polymode    = PolygonMode::Fill;
            FrontFace                   front       = FrontFace::CounterClockwise;
            CullMode                    culling     = CullMode::Back;
            PushConfig                  push        = {};
        };
    }
}
