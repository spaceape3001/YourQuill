////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include "StdObject.hpp"
#include <yq/keyv/KeyValue.hpp>
#include <yq/meta/Init.hpp>
#include <yq/stream/Text.hpp>

namespace yq::mithril {
    Ref<StdObject>   StdObject::load(const KeyValue& keyvalue, const ObjectMeta& base)
    {
        const ObjectMeta*   obj = ObjectMeta::find(keyvalue.data);
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
        const ObjectMeta&   info = metaInfo();
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
        const ObjectMeta&   info = metaInfo();
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

YQ_OBJECT_IMPLEMENT(yq::mithril::StdObject)
