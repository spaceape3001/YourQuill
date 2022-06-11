////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

namespace yq {
    namespace engine {
        enum class PushConstant : uint8_t {
            //! No push constants (default)
            None            = 0,
            //! Model/View/Projection combined into a single 4x4 (float)
            FullMatrix,
            //! Model/View/Projection combined into a single 4x4 (double)
            FullMatrix64,
            
            //! View/Projection combined into a single 4x4 (float)
            World,
            
            //! View/Projection combined into a single 4x4 (double)
            World64,
            
            //! View & Projection as separate 4x4 matrices
            ViewProj
        };
    }
}
