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

#include "shape2/Py2Point.ipp"
#include "shape3/Py3Point.ipp"
#include "shape4/Py4Point.ipp"

#include <0/basic/DelayInit.hpp>
#include <0/meta/Init.hpp>


YQ_TYPE_IMPLEMENT(yq::doodle::D)

YQ_OBJECT_IMPLEMENT(yq::doodle::Association)
YQ_OBJECT_IMPLEMENT(yq::doodle::Camera)
YQ_OBJECT_IMPLEMENT(yq::doodle::Constraint)
YQ_OBJECT_IMPLEMENT(yq::doodle::DObject)
YQ_OBJECT_IMPLEMENT(yq::doodle::Light)
YQ_OBJECT_IMPLEMENT(yq::doodle::Motion)
YQ_OBJECT_IMPLEMENT(yq::doodle::Py)
YQ_OBJECT_IMPLEMENT(yq::doodle::Py1)
YQ_OBJECT_IMPLEMENT(yq::doodle::Py2)
YQ_OBJECT_IMPLEMENT(yq::doodle::Py2Point)
YQ_OBJECT_IMPLEMENT(yq::doodle::Py3)
YQ_OBJECT_IMPLEMENT(yq::doodle::Py3Point)
YQ_OBJECT_IMPLEMENT(yq::doodle::Py4)
YQ_OBJECT_IMPLEMENT(yq::doodle::Py4Point)
YQ_OBJECT_IMPLEMENT(yq::doodle::Space)
YQ_OBJECT_IMPLEMENT(yq::doodle::Space2)

namespace {
    void    reg_me()
    {
        using namespace yq;
        using namespace yq::doodle;
        
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
