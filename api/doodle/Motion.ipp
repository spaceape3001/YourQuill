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

    Motion* MotionInfo::createM(DObject* parent) const
    {
        return static_cast<Motion*>(createD(parent));
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////


    Motion::Motion(DObject* parent) : DObject(parent)
    {
    }
    
    Motion::Motion(const Motion&cp) : DObject(cp)
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

YQ_OBJECT_IMPLEMENT(yq::doodle::Motion)
