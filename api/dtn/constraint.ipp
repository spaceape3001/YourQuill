////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "constraint.hpp"
#include <yq/io/Strings.hpp>
#include <yq/meta/ObjectInfoWriter.hpp>
#include <yq/io/XmlUtils.hpp>

namespace yq {

    Ref<Constraint>     x_constraint(const XmlNode* xn)
    {
        std::string     s   = read_attribute(xn, szType, x_sstring);
        const ObjectInfo*   oi  = ObjectInfo::find(s);
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

