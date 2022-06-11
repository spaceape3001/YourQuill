////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/shader/ShaderPointer.hpp>
#include <optional>
#include <vector>
#include "CullMode.hpp"
#include "PolygonMode.hpp"
#include "Topology.hpp"
#include "FrontFace.hpp"
#include "DataFormat.hpp"
#include "VertexInputRate.hpp"

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
        
        struct PipelineConfig {
            std::vector<ShaderSpec>     shaders;
            std::vector<VBOConfig>      vbos;
            Topology                    topology    = Topology::TriangleList;
            PolygonMode                 polymode    = PolygonMode::Fill;
            FrontFace                   front       = FrontFace::CounterClockwise;
            CullMode                    culling     = CullMode::Back;
        };
    }
}
