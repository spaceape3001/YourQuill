////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Motion.hpp"
#include "MotionInfoWriter.hpp"

YQ_OBJECT_IMPLEMENT(yq::doodler::Motion)

namespace yq::doodler {
    MotionInfo::MotionInfo(std::string_view zName, DObjectInfo& base, const std::source_location& sl) : DObjectInfo(zName, base, sl)
    {
        set(Flag::MOTION);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    void Motion::init_info()
    {
        auto w = writer<Motion>();
        w.description("Motion");
    }

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

