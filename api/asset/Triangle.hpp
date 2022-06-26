////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/render/Render3D.hpp>
#include <array>

namespace yq {
    namespace asset {
        class Triangle : public engine::Render3D {
            YQ_OBJECT_DECLARE(Triangle, engine::Render3D)
        public:
        
            Triangle(const Triangle3D&, const TriangleData<RGBA4F>&);
            ~Triangle();
            
            static void initInfo();
            
        private:
            std::array<glm::vec3,3>    m_positions;
            std::array<glm::vec4,3>    m_colors;
        };
    }
}

