////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "StdObject.hpp"
#include <mithril/kernel/bit/KeyValue.hpp>
#include <meta/Init.hpp>
#include <io/stream/Text.hpp>

namespace yq {
    Ref<StdObject>   StdObject::load(const KeyValue& keyvalue, const ObjectInfo& base)
    {
        const ObjectInfo*   obj = ObjectInfo::find(keyvalue.data);
        if(!obj)
            return Ref<StdObject>();
        if(obj->is_abstract() || !obj->is_base(base))
            return Ref<StdObject>();
        
        Ref<StdObject> ret = static_cast<StdObject*>(obj -> create());
        if(!ret)
            return Ref<StdObject>();
        if(!ret -> load(keyvalue))
            return Ref<StdObject>();
        return ret;
    }

    bool    StdObject::read(const KeyValue& kvs)
    {
        const ObjectInfo&   info = metaInfo();
        for(auto * pi : info.properties(true).all){
            if(pi && pi->is_state() && !pi->is_static() && pi->setter()){
                const KeyValue* a   = kvs.first(pi->name());
                if(!a)
                    continue;
                if(!pi -> set(this, a->data))
                    yInfo() << "Property " << pi->name() << " failed to set.";
            }
        }
        return true;
    }
    
    bool    StdObject::save(KeyValue& kvs) const
    {
        const ObjectInfo&   info = metaInfo();
        kvs.data        = info.name();
        
        for(auto * pi : info.properties(true).all){
            if(pi && pi->is_state() && !pi->is_static() && pi->setter()){
                KeyValue    kv;
                kv.key      = pi -> name();
                stream::Text    text(kv.data);
                pi -> write(this, text);
            }
        }
        return true;
    }
        
    StdObject::StdObject()
    {
    }
    
    StdObject::~StdObject()
    {
    }
}

YQ_OBJECT_IMPLEMENT(yq::StdObject)
