////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Constraint.hpp"
#include <aether/kernel/io/Strings.hpp>
#include <meta/ObjectInfoWriter.hpp>
#include <io/XmlUtils.hpp>

namespace yq {

    Ref<Constraint>     x_constraint(const XmlNode* xn)
    {
        const ObjectInfo*   oi  = ObjectInfo::find(read_attribute(xn, szType, x_string));
        if(!oi)
            return Ref<Constraint>();
        Object* obj         = oi->create();
        Ref<Constraint>     auth    = dynamic_cast<Constraint*>(obj);
        if(auth && auth->load(xn))
            return auth;
        if(obj)
            delete obj;
        return Ref<Constraint>();
    }
    
    void    write_xn(XmlNode* xn, const Ref<Constraint>&v)
    {
        if(v.valid()){
            write_attribute(xn, szType, v->metaInfo().name());
            v->save(xn);
        }
    }
}

YQ_OBJECT_IMPLEMENT(yq::Constraint)

