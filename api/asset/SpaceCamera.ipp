////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "SpaceCamera.hpp"
#include <glm/ext/matrix_clip_space.hpp>

namespace yq {
    SpaceCamera::SpaceCamera()
    {
    }
    
    SpaceCamera::~SpaceCamera()
    {
    }
    
    glm::dmat4  SpaceCamera::projection_matrix(const Size2D&sz) const
    {
        return glm::perspectiveFovNO<double>( glm::radians(m_fov.value), sz.width(), sz.height(), m_near, m_far);
    }

    void        SpaceCamera::set_far(double v)
    {
        m_far   = v;
        changed();
    }
    
    void        SpaceCamera::set_near(double v)
    {
        m_near  = v;
        changed();
    }
    
    void        SpaceCamera::set_orientation(const Quaternion3D&v)
    {
        m_space.orientation = v;
        changed();
    }
    
    void        SpaceCamera::set_position(const Vector3D&v)
    {
        m_space.position    = v;
        changed();
    }
    
    void        SpaceCamera::set_scale(const Vector3D&v)
    {
        m_space.scale       = v;
        changed();
    }
    
    void        SpaceCamera::set_fov(Degree v)
    {
        m_fov = v;
        changed();
    }

    glm::dmat4  SpaceCamera::view_matrix() const
    {
        return m_space.parent2local();
    }

    glm::dmat4  SpaceCamera::world2screen(const CameraParams&p) const
    {
        return view_matrix() * projection_matrix(p.screen);
    }
}
YQ_OBJECT_IMPLEMENT(yq::SpaceCamera)
