////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/Py3.hpp>
#include <yq/vector/Vector3.hpp>

namespace yq::doodler {
    class Py3Point : public Py3, public Py3::Position {
        YQ_OBJECT_DECLARE(Py3Point, Py3)
    public:
    
        Py3Point(Project&);
        Py3Point(Project&, const Py3Point&);
        ~Py3Point();
        
        virtual Vector3D    get_position() const override { return m_point; }
        virtual void        set_position(const Vector3D&) override;
        
        const Vector3D& point() const { return m_point; }
        void            set_point(const Vector3D&);
        
        
    private:
        Vector3D        m_point = {};
    };
}

