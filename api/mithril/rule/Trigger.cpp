////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/rule/Trigger.hpp>
#include <mithril/io/Strings.hpp>
#include <yq/xml/XmlUtils.hpp>
#include <yq/meta/ObjectInfoWriter.hpp>

YQ_OBJECT_IMPLEMENT(yq::mithril::Trigger)

namespace yq::mithril {
    Trigger::Trigger()
    {
    }
    
    Trigger::~Trigger()
    {
    }
    
    void Trigger::init_meta()
    {
        auto w = writer<Trigger>();
        w.description("Trigger");
    }

    Ref<Trigger>     x_trigger(const XmlNode& xn)
    {
        const ObjectInfo*   oi  = ObjectInfo::find(read_attribute(xn, szType, x_string));
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
    
    void    write_xn(XmlNode& xn, const Ref<Trigger>&v)
    {
        if(v.valid()){
            write_attribute(xn, szType, v->metaInfo().name());
            v->save(xn);
        }
    }
}

