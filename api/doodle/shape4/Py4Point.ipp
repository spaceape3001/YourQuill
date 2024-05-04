////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodle/shape4/Py4Point.hpp>

namespace yq::doodle {
    Py4Point::Py4Point(Project&prj) : Py4(prj)
    {
    }
    
    Py4Point::Py4Point(Project&prj, const Py4Point&cp) : Py4(prj, cp), m_point(cp.m_point)
    {
    }
    
    Py4Point::~Py4Point()
    {
    }
    
    void   Py4Point::set_position(const Vector4D&v)
    {
        set_point(v);
    }
    
    void   Py4Point::set_point(const Vector4D&v)
    {
        m_point = v;
        bump();
    }
}
