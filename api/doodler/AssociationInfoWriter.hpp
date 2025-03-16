////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/DObjectInfoWriter.hpp>
#include <doodler/Association.hpp>

namespace yq::doodler {
    template <typename T>
    class AssociationInfo::Writer : public DObjectInfo::Writer<T> {
    public:
        Writer(AssociationInfo* aInfo) : DObjectInfo::Writer<T>(aInfo)
        {
        }
        
        Writer(AssociationInfo& aInfo) : Writer(&aInfo)
        {
        }
    };

}
