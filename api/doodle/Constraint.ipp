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

    Constraint* ConstraintInfo::createC(DObject* parent) const
    {
        return static_cast<Constraint*>(createD(parent));
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////


    Constraint::Constraint(DObject* parent) : DObject(parent)
    {
    }
    
    Constraint::Constraint(const Constraint&cp) : DObject(cp)
    {
    }
    
    Constraint::~Constraint()
    {
    }



    //! Remap IDs/pointers appropriately
    void        Constraint::remap(const Remapper&rMap)
    {
        DObject::remap(rMap);
    }
}

YQ_OBJECT_IMPLEMENT(yq::doodle::Constraint)
