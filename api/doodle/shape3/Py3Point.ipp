////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodle/shape3/Py3Point.hpp>

namespace yq::doodle {
    Py3Point::Py3Point(Project&prj) : Py3(prj)
    {
    }
    
    Py3Point::Py3Point(Project&prj, const Py3Point&cp) : Py3(prj, cp), m_point(cp.m_point)
    {
    }
    
    Py3Point::~Py3Point()
    {
    }
    
    void   Py3Point::set_position(const Vector3D&v)
    {
        set_point(v);
    }
    
    void   Py3Point::set_point(const Vector3D&v)
    {
        m_point = v;
        bump();
    }
}
