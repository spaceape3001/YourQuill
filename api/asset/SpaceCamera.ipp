////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "SpaceCamera.hpp"
#include <glm/ext/matrix_clip_space.hpp>

namespace yq {
    SpaceCamera::SpaceCamera() : 
        m_space(this),
        m_fov(70_deg, this),
        m_near(-1., this),
        m_far(1., this)
    {
    }
    
    SpaceCamera::~SpaceCamera()
    {
    }
    
    glm::dmat4  SpaceCamera::projection_matrix(const Size2D&sz) const
    {
        return glm::perspectiveFovNO<double>( glm::radians(m_fov->value), sz.width(), sz.height(), m_near, m_far);
    }

    void        SpaceCamera::set_far(double v)
    {
        m_far   = v;
    }
    
    void        SpaceCamera::set_near(double v)
    {
        m_near  = v;
    }
    
    void        SpaceCamera::set_orientation(const Quaternion3D&v)
    {
        m_space.edit().orientation = v;
    }
    
    void        SpaceCamera::set_position(const Vector3D&v)
    {
        m_space.edit().position    = v;
    }
    
    void        SpaceCamera::set_scale(const Vector3D&v)
    {
        m_space.edit().scale       = v;
    }
    
    void        SpaceCamera::set_fov(Degree v)
    {
        m_fov = v;
    }

    glm::dmat4  SpaceCamera::view_matrix() const
    {
        return m_space->parent2local();
    }

    glm::dmat4  SpaceCamera::world2screen(const CameraParams&p) const
    {
        return view_matrix() * projection_matrix(p.screen);
    }
}
YQ_OBJECT_IMPLEMENT(yq::SpaceCamera)
