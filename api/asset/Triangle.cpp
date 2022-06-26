////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Triangle.hpp"
#include <math/Triangle3.hpp>
#include <math/shape_math.hpp>
#include <engine/render/RenderWriter.hpp>
#include <engine/shader/Shader.hpp>
#include <basic/preamble.hpp>

namespace yq {
    namespace asset {
        void Triangle::initInfo()
        {
            auto w = writer<Triangle>();
            auto p = w.pipeline();
            
            p.shader("assets/triangle.vert");
            p.shader("assets/triangle.frag");
            
            p.static_vertex(&Triangle::m_positions, "positions"sv);
            p.static_vertex(&Triangle::m_colors, "colors"sv);
            
            p.push(yq::engine::PushConfigType::Full);
        }

        Triangle::Triangle(const Triangle3D&tri, const TriangleData<RGBA4F>& colors)
        {
            m_positions[0]  = (glm::dvec3) tri.a;
            m_positions[1]  = (glm::dvec3) tri.b;
            m_positions[2]  = (glm::dvec3) tri.c;

            m_colors[0]     = colors.a;
            m_colors[1]     = colors.b;
            m_colors[2]     = colors.c;
        }
        
        Triangle::~Triangle()
        {
        }
        
        YQ_INVOKE(
            Triangle::initInfo();
        );
    }
}
YQ_OBJECT_IMPLEMENT(yq::asset::Triangle)
