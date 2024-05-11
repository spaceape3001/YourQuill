////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Light.hpp"

namespace yq::doodler {
    LightInfo::LightInfo(std::string_view zName, const DObjectInfo& base, const std::source_location& sl) : DObjectInfo(zName, base, sl)
    {
        set(Flag::LIGHT);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////


    Light::Light(Project& prj) : DObject(prj)
    {
    }
    
    Light::Light(Project& prj, const Light& cp) : DObject(prj, cp)
    {
    }
    
    Light::~Light()
    {
    }



    //! Remap IDs/pointers appropriately
    void        Light::remap(const Remapper&rMap)
    {
        DObject::remap(rMap);
    }
}

