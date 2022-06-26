////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/render/Render3D.hpp>
#include <array>
#include <math/ColorVertexData.hpp>

namespace yq {
    namespace asset {
        class ColorQuadrilateral : public engine::Render3D {
            YQ_OBJECT_DECLARE(ColorQuadrilateral, engine::Render3D)
        public:
        
            ColorQuadrilateral(const QuadrilateralData<ColorVertex2D>&);
            ~ColorQuadrilateral();
            
            static void initInfo();
            
        private:
            std::array<ColorVertexData,4>   m_vertex;
            std::array<uint16_t,6>          m_index;
        };
    }
}

