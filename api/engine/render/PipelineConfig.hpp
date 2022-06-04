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

namespace yq {
    namespace engine {
        struct PipelineConfig {
            std::vector<ShaderSpec>     shaders;
            Topology                    topology    = Topology::TriangleList;
            PolygonMode                 polymode    = PolygonMode::Fill;
            FrontFace                   front       = FrontFace::CounterClockwise;
            CullMode                    cull        = CullMode::Back;
        };
    }
}
