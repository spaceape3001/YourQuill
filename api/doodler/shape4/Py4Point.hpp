////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/Py4.hpp>
#include <yq-toolbox/vector/Vector4.hpp>

namespace yq::doodler {
    class Py4Point : public Py4, public Py4::Position {
        YQ_OBJECT_DECLARE(Py4Point, Py4)
    public:
    
        Py4Point(Project&);
        Py4Point(Project&, const Py4Point&);
        ~Py4Point();
        
        virtual Vector4D    get_position() const override { return m_point; }
        virtual void        set_position(const Vector4D&) override;
        
        const Vector4D& point() const { return m_point; }
        void            set_point(const Vector4D&);
        
        
    private:
        Vector4D        m_point = {};
    };
}

