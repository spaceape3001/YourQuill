////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Constraint.hpp"

namespace yq::doodle {
    ConstraintInfo::ConstraintInfo(std::string_view zName, const DObjectInfo& base, const std::source_location& sl) : DObjectInfo(zName, base, sl)
    {
        set(Flag::CONSTRAINT);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////


    Constraint::Constraint(Project&prj) : DObject(prj)
    {
    }
    
    Constraint::Constraint(Project& prj, const Constraint& cp) : DObject(prj, cp)
    {
    }
    
    Constraint::~Constraint()
    {
    }


    void        Constraint::remap(const Remapper&rMap)
    {
        DObject::remap(rMap);
    }
}

