////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Action.hpp"
#include <kernel/io/Strings.hpp>
#include <basic/meta/ObjectInfoWriter.hpp>
#include <basic/XmlUtils.hpp>

namespace yq {

    Ref<Action>     x_action(const XmlNode* xn)
    {
        const ObjectInfo*   oi  = ObjectInfo::find(read_attribute(xn, szType, x_string));
        if(!oi)
            return Ref<Action>();
        Object* obj         = oi->create();
        Ref<Action>     auth    = dynamic_cast<Action*>(obj);
        if(auth && auth->load(xn))
            return auth;
        if(obj)
            delete obj;
        return Ref<Action>();
    }
    
    void    write_xn(XmlNode* xn, const Ref<Action>&v)
    {
        if(v.valid()){
            write_attribute(xn, szType, v->metaInfo().name());
            v->save(xn);
        }
    }
}

YQ_OBJECT_IMPLEMENT(yq::Action)

