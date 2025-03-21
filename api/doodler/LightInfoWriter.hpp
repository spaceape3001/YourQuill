////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/DObjectInfoWriter.hpp>
#include <doodler/Light.hpp>

namespace yq::doodler {

    template <typename T>
    class LightInfo::Writer : public DObjectInfo::Writer<T> {
    public:
        Writer(LightInfo* lInfo) : DObjectInfo::Writer<T>(lInfo)
        {
        }
        
        Writer(LightInfo& lInfo) : Writer(&lInfo)
        {
        }
    };

}
