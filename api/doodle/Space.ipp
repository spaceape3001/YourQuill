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

    Space* SpaceInfo::createS(DObject* parent) const
    {
        return static_cast<Space*>(createD(parent));
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////


    Space::Space(DObject* parent) : DObject(parent)
    {
    }
    
    Space::Space(const Space&cp) : DObject(cp)
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

YQ_OBJECT_IMPLEMENT(yq::doodle::Space)
