////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "action.hpp"
#include <yq/meta/ObjectInfoWriter.hpp>

namespace yq {

    Ref<Action>     x_action(const XmlNode* xn)
    {
        std::string     s   = read_attribute(xn, szType, x_sstring);
        const ObjectInfo*   oi  = ObjectInfo::find(s);
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

