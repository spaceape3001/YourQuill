////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Space.hpp"

namespace yq::doodle {
    SpaceInfo::SpaceInfo(std::string_view zName, const DObjectInfo& base, const std::source_location& sl) : DObjectInfo(zName, base, sl)
    {
        set(Flag::SPACE);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    Space::Space(Project&prj) : DObject(prj)
    {
    }
    
    Space::Space(Project&prj, const Space&cp) : DObject(prj, cp)
    {
    }

    Space::~Space()
    {
    }


    //! Remap IDs/pointers appropriately
    void        Space::remap(const Remapper&rMap)
    {
        DObject::remap(rMap);
    }
}

