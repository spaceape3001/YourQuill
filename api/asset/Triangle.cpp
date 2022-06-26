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

            p.static_vertex(&Triangle::m_vertex, "vertex"sv)
                .attribute(&Vertex::position)
                .attribute(&Vertex::color)
            ;
            
            p.push(yq::engine::PushConfigType::Full);
        }

        Triangle::Triangle(const Triangle3D&tri, const TriangleData<RGBA4F>& colors)
        {
            m_vertex[0] = { (glm::dvec3) tri.a, colors.a };
            m_vertex[1] = { (glm::dvec3) tri.b, colors.b };
            m_vertex[2] = { (glm::dvec3) tri.c, colors.c };
            m_draw.vertex_count  = 3;
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
