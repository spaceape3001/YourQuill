////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/rule/Action.hpp>
#include <mithril/io/Strings.hpp>
#include <yq/xml/XmlUtils.hpp>
#include <yq/meta/ObjectMetaWriter.hpp>

YQ_OBJECT_IMPLEMENT(yq::mithril::Action)

namespace yq::mithril {
    Action::Action()
    {
    }
    
    Action::~Action()
    {
    }
    
    void Action::init_meta()
    {
        auto w = writer<Action>();
        w.description("Action");
    }

    Ref<Action>     x_action(const XmlNode& xn)
    {
        const ObjectMeta*   oi  = ObjectMeta::find(read_attribute(xn, szType, x_string));
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
    
    void    write_xn(XmlNode& xn, const Ref<Action>&v)
    {
        if(v.valid()){
            write_attribute(xn, szType, v->metaInfo().name());
            v->save(xn);
        }
    }
}


