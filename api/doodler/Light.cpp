////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Light.hpp"
#include "LightInfoWriter.hpp"

YQ_OBJECT_IMPLEMENT(yq::doodler::Light)

namespace yq::doodler {
    LightInfo::LightInfo(std::string_view zName, DObjectInfo& base, const std::source_location& sl) : DObjectInfo(zName, base, sl)
    {
        set(Flag::LIGHT);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    void Light::init_info()
    {
        auto w = writer<Light>();
        w.description("Light");
    }

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

