////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/shader/ShaderPointer.hpp>
#include <optional>
#include <vector>

namespace yq {
    namespace engine {
        struct PipelineConfig {
            std::vector<ShaderPtr>    shaders;
        };
    }
}
