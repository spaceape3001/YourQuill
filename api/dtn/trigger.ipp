////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "trigger.hpp"
#include <yq/io/Strings.hpp>
#include <yq/meta/ObjectInfoWriter.hpp>
#include <yq/io/XmlUtils.hpp>

namespace yq {

    Ref<Trigger>     x_trigger(const XmlNode* xn)
    {
        std::string     s   = read_attribute(xn, szType, x_sstring);
        const ObjectInfo*   oi  = ObjectInfo::find(s);
        if(!oi)
            return Ref<Trigger>();
        Object* obj         = oi->create();
        Ref<Trigger>     trig    = dynamic_cast<Trigger*>(obj);
        if(trig && trig->load(xn))
            return trig;
        if(obj)
            delete obj;
        return Ref<Trigger>();
    }
    
    void    write_xn(XmlNode* xn, const Ref<Trigger>&v)
    {
        if(v.valid()){
            write_attribute(xn, szType, v->metaInfo().name());
            v->save(xn);
        }
    }
}

YQ_OBJECT_IMPLEMENT(yq::Trigger)

