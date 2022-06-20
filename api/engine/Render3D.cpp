////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Render3D.hpp"
#include "Render3DInfoWriter.hpp"
#include <basic/DelayInit.hpp>
#include <basic/meta/Init.hpp>
#include <atomic>

namespace yq {
    namespace engine {

        Render3DInfo::Render3DInfo(std::string_view name, ObjectInfo& base, const std::source_location& sl) : 
            RenderedInfo(name, base, sl)
        {
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Render3D::Render3D()    = default;
        Render3D::~Render3D()   = default;

        Tensor44D   Render3D::calc_local() const
        {
            return m_space.value.local2parent();
        }

        void        Render3D::set_bounds(const AxBox3D&v)
        {
            m_bounds    = v;
            changed();
        }

        void        Render3D::set_orientation(const Quaternion3D&v)
        {
            m_space.value.orientation   = v;
            m_space.dirty               = true;
            changed();
        }

        void        Render3D::set_position(const Vector3D&v)
        {
            m_space.value.position      = v;
            m_space.dirty               = true;
            changed();
        }
        
        void        Render3D::set_scale(const Vector3D&v)
        {
            m_space.value.scale         = v;
            m_space.dirty               = true;
            changed();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        YQ_INVOKE(
            auto robj   = writer<Render3D>();
            robj.property("pos", &Render3D::position).setter(&Render3D::set_position);
            robj.property("scale", &Render3D::scale).setter(&Render3D::set_scale);
            robj.property("ori", &Render3D::orientation).setter(&Render3D::set_orientation);
            robj.property("bounds", &Render3D::bounds).setter(&Render3D::set_bounds);
        )
    }
}

YQ_OBJECT_IMPLEMENT(yq::engine::Render3D)
