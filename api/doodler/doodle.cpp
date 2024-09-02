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
#include "Project.ipp"
#include "Py.ipp"
#include "Py1.ipp"
#include "Py2.ipp"
#include "Py3.ipp"
#include "Py4.ipp"
#include "Space.ipp"
#include "Space2.ipp"

#include "b3/grammar.ipp"

#include "shape2/Py2Point.ipp"
#include "shape3/Py3Point.ipp"
#include "shape4/Py4Point.ipp"

#include <yq/basic/DelayInit.hpp>
#include <yq/meta/Init.hpp>


YQ_TYPE_IMPLEMENT(yq::doodler::ID)

YQ_OBJECT_IMPLEMENT(yq::doodler::Association)
YQ_OBJECT_IMPLEMENT(yq::doodler::Camera)
YQ_OBJECT_IMPLEMENT(yq::doodler::Constraint)
YQ_OBJECT_IMPLEMENT(yq::doodler::DObject)
YQ_OBJECT_IMPLEMENT(yq::doodler::Light)
YQ_OBJECT_IMPLEMENT(yq::doodler::Motion)
YQ_OBJECT_IMPLEMENT(yq::doodler::Py)
YQ_OBJECT_IMPLEMENT(yq::doodler::Py1)
YQ_OBJECT_IMPLEMENT(yq::doodler::Py2)
YQ_OBJECT_IMPLEMENT(yq::doodler::Py2Point)
YQ_OBJECT_IMPLEMENT(yq::doodler::Py3)
YQ_OBJECT_IMPLEMENT(yq::doodler::Py3Point)
YQ_OBJECT_IMPLEMENT(yq::doodler::Py4)
YQ_OBJECT_IMPLEMENT(yq::doodler::Py4Point)
YQ_OBJECT_IMPLEMENT(yq::doodler::Space)
YQ_OBJECT_IMPLEMENT(yq::doodler::Space2)

namespace {
    void    reg_me()
    {
        using namespace yq;
        using namespace yq::doodler;
        
        {
            auto w = writer<DObject>();
            w.property<std::string>("notes", &DObject::notes).setter(&DObject::set_notes);
            w.property<std::string>("title", &DObject::title).setter(&DObject::set_title);
        }
        
        {
            auto w = writer<Py2Point>();
            w.property<Vector2D>("point", &Py2Point::point).setter(&Py2Point::set_point);
        }
        
        {
            auto w = writer<Py3Point>();
            w.property<Vector3D>("point", &Py3Point::point).setter(&Py3Point::set_point);
        }

        {
            auto w = writer<Py4Point>();
            w.property<Vector4D>("point", &Py4Point::point).setter(&Py4Point::set_point);
        }
    }
    
    YQ_INVOKE(reg_me();)
}
