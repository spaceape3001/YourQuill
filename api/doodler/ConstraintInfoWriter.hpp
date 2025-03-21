////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/DObjectInfoWriter.hpp>
#include <doodler/Constraint.hpp>

namespace yq::doodler {
    template <typename T>
    class ConstraintInfo::Writer : public DObjectInfo::Writer<T> {
    public:
        Writer(ConstraintInfo* cInfo) : DObjectInfo::Writer<T>(cInfo)
        {
        }
        
        Writer(ConstraintInfo& cInfo) : Writer(&cInfo)
        {
        }
    };
}
