////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Space2.hpp"

namespace yq::doodler {
    Space2Info::Space2Info(std::string_view zName, const SpaceInfo& base, const std::source_location& sl) : SpaceInfo(zName, base, sl)
    {
        set(Flag::SPACE);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    Space2::Space2(Project&prj) : Space(prj)
    {
    }
    
    Space2::Space2(Project&prj, const Space2&cp) : Space(prj, cp)
    {
    }

    Space2::~Space2()
    {
    }


    //! Remap IDs/pointers appropriately
    void        Space2::remap(const Remapper&rMap)
    {
        Space::remap(rMap);
    }
}

