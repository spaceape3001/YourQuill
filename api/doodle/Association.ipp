////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Association.hpp"

namespace yq::doodle {
    AssociationInfo::AssociationInfo(std::string_view zName, const DObjectInfo& base, const std::source_location& sl) : DObjectInfo(zName, base, sl)
    {
        set(Flag::ASSOCIATION);
    }

    Association* AssociationInfo::createA(DObject* parent) const
    {
        return static_cast<Association*>(createD(parent));
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////


    Association::Association(DObject* parent) : DObject(parent)
    {
    }
    
    Association::Association(const Association&cp) : DObject(cp)
    {
    }
    
    Association::~Association()
    {
    }



    //! Remap IDs/pointers appropriately
    void        Association::remap(const Remapper&rMap)
    {
        DObject::remap(rMap);
    }
}

YQ_OBJECT_IMPLEMENT(yq::doodle::Association)
