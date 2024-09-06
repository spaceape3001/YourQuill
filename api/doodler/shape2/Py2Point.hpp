////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/Py2.hpp>
#include <yq-toolbox/vector/Vector2.hpp>

namespace yq::doodler {
    class Py2Point : public Py2, public Py2::Position {
        YQ_OBJECT_DECLARE(Py2Point, Py2)
    public:
    
        Py2Point(Project&);
        Py2Point(Project&, const Py2Point&);
        ~Py2Point();
        
        virtual Vector2D    get_position() const override { return m_point; }
        virtual void        set_position(const Vector2D&) override;
        
        const Vector2D& point() const { return m_point; }
        void            set_point(const Vector2D&);
        
        
    private:
        Vector2D        m_point = {};
    };
}

