////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Motion.hpp"

namespace yq::doodle {
    MotionInfo::MotionInfo(std::string_view zName, const DObjectInfo& base, const std::source_location& sl) : DObjectInfo(zName, base, sl)
    {
        set(Flag::MOTION);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////


    Motion::Motion(Project& prj) : DObject(prj)
    {
    }
    
    Motion::Motion(Project& prj, const Motion& cp) : DObject(prj, cp)
    {
    }
    
    Motion::~Motion()
    {
    }

    //! Remap IDs/pointers appropriately
    void        Motion::remap(const Remapper&rMap)
    {
        DObject::remap(rMap);
    }
}

