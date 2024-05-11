////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/shape2/Py2Point.hpp>

namespace yq::doodler {
    Py2Point::Py2Point(Project&prj) : Py2(prj)
    {
    }
    
    Py2Point::Py2Point(Project&prj, const Py2Point&cp) : Py2(prj, cp), m_point(cp.m_point)
    {
    }
    
    Py2Point::~Py2Point()
    {
    }
    
    void   Py2Point::set_position(const Vector2D&v)
    {
        set_point(v);
    }
    
    void   Py2Point::set_point(const Vector2D&v)
    {
        m_point = v;
        bump();
    }
}
