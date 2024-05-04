////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Light.hpp"

namespace yq::doodle {
    LightInfo::LightInfo(std::string_view zName, const DObjectInfo& base, const std::source_location& sl) : DObjectInfo(zName, base, sl)
    {
        set(Flag::LIGHT);
    }

    Light* LightInfo::createC(DObject* parent) const
    {
        return static_cast<Light*>(createD(parent));
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////


    Light::Light(DObject* parent) : DObject(parent)
    {
    }
    
    Light::Light(const Light&cp) : DObject(cp)
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

YQ_OBJECT_IMPLEMENT(yq::doodle::Light)
