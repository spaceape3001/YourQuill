////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/Render3D.hpp>
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
            struct Vertex {
                glm::vec4   color;
                glm::vec3   position;
            };
            
            std::array<Vertex,3>    m_corners;
        };
    }
}

