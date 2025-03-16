////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/DObjectInfoWriter.hpp>
#include <doodler/Camera.hpp>

namespace yq::doodler {
    template <typename T>
    class CameraInfo::Writer : public DObjectInfo::Writer<T> {
    public:
        Writer(CameraInfo* cInfo) : DObjectInfo::Writer<T>(cInfo)
        {
        }
        
        Writer(CameraInfo& cInfo) : Writer(&cInfo)
        {
        }
    };
}
