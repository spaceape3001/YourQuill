////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Triangle.hpp"
#include <math/Triangle3.hpp>
#include <math/shape_math.hpp>
#include <engine/render/Render3DInfoWriter.hpp>
#include <engine/shader/Shader.hpp>

namespace yq {
    namespace asset {
        void Triangle::initInfo()
        {
            auto w = writer<Triangle>();
            auto p = w.pipeline();
            p.shader("assets/triangle.vert");
            p.shader("assets/triangle.frag");
            
            //  TODO
        }

        Triangle::Triangle(const Triangle3D&tri, const TriangleData<RGBA4F>& colors)
        {
            m_corners[0].position   = (glm::dvec3) tri.a;
            m_corners[0].color      = colors.a;
            m_corners[1].position   = (glm::dvec3) tri.b;
            m_corners[1].color      = colors.b;
            m_corners[2].position   = (glm::dvec3) tri.c;
            m_corners[2].color      = colors.c;
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
