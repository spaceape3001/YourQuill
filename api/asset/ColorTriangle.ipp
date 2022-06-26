////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ColorTriangle.hpp"
#include <math/TriangleData.hpp>
#include <math/shape_math.hpp>
#include <engine/render/RenderWriter.hpp>
#include <engine/shader/Shader.hpp>
#include <basic/preamble.hpp>

namespace yq {
    namespace asset {
        void ColorTriangle::initInfo()
        {
            auto w = writer<ColorTriangle>();
            auto p = w.pipeline();
            
            p.shader("assets/colored.vert");
            p.shader("assets/colored.frag");

            p.static_vertex(&ColorTriangle::m_vertex, "vertex"sv)
                .attribute(&ColorVertexData::position)
                .attribute(&ColorVertexData::color)
            ;
            
            p.push(yq::engine::PushConfigType::Full);
        }

        ColorTriangle::ColorTriangle(const TriangleData<ColorVertex2D>&tri)
        {
            m_vertex[0] = tri.a;
            m_vertex[1] = tri.b;
            m_vertex[2] = tri.c;
            m_draw.vertex_count  = 3;
        }
        
        ColorTriangle::~ColorTriangle()
        {
        }
        
        YQ_INVOKE(
            ColorTriangle::initInfo();
        );
    }
}
YQ_OBJECT_IMPLEMENT(yq::asset::ColorTriangle)
