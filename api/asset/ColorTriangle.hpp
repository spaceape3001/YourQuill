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
        class ColorTriangle : public engine::Render3D {
            YQ_OBJECT_DECLARE(ColorTriangle, engine::Render3D)
        public:
        
            ColorTriangle(const TriangleData<ColorVertex2D>&);
            ~ColorTriangle();
            
            static void initInfo();
            
        private:
        
            std::array<ColorVertexData,3>        m_vertex;
        };
    }
}

