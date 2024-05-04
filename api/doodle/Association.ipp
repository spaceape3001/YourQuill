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

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    Association::Association(Project& prj) : DObject(prj)
    {
    }
    
    Association::Association(Project& prj, const Association& assn) : DObject(prj, assn)
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

