////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/DObjectInfoWriter.hpp>
#include <doodler/Motion.hpp>

namespace yq::doodler {
    template <typename T>
    class MotionInfo::Writer : public DObjectInfo::Writer<T> {
    public:
        Writer(MotionInfo* mInfo) : DObjectInfo::Writer<T>(mInfo)
        {
        }
        
        Writer(MotionInfo& mInfo) : Writer(&mInfo)
        {
        }
    };
}
