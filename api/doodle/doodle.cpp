////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include "Association.ipp"
#include "Camera.ipp"
#include "Constraint.ipp"
#include "DObject.ipp"
#include "Light.ipp"
#include "Motion.ipp"
#include "Py.ipp"
#include "Space.ipp"

#include <0/basic/DelayInit.hpp>

namespace {
    void    reg_me()
    {
        using namespace yq::doodle;
        reg_dobject();
    }
    
    YQ_INVOKE(reg_me();)
}

