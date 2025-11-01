////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/rule/Rule.hpp>
#include <mithril/io/Strings.hpp>
#include <yq/xml/XmlUtils.hpp>
#include <yq/meta/ObjectMetaWriter.hpp>

YQ_OBJECT_IMPLEMENT(yq::mithril::Rule)

namespace yq::mithril {    
    Rule::Rule()
    {
    }
    
    Rule::~Rule()
    {
    }

    void Rule::init_meta()
    {
        auto w = writer<Rule>();
        w.label("Generic Rule");
        w.property("name", &Rule::name);
    }

/*
    Rule&       Rule::merge(const Rule&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        actions += b.actions;
        constraints += b.constraints;
        triggers += b.triggers;
        return *this;
    }
    
    Rule        x_rule(const XmlNode*xn)
    {
        Rule    ret;
        read_xn(ret, xn);
        return ret;
    }
    
    bool        read_xn(Rule&ret, const XmlNode*xn)
    {
        ret.name        = read_child(xn, szName, x_string);
        ret.actions     = read_children(xn, szAction, x_action);
        ret.constraint  = read_children(xn, szConstraint, x_constraint);
        ret.triggers    = read_children(xn, szTrigger, x_trigger);
        return true;
    }
    
    void        write_xn(XmlNode*xn, const Rule& v)
    {
        write_child(xn, szName, v.name);
        write_children(xn, szTrigger, v.triggers);
        write_children(xn, szConstraint, v.constraints);
        write_children(xn, szAction, v.actions);
    }
*/
    Ref<Rule>     x_rule(const XmlNode& xn)
    {
        const ObjectMeta*   oi  = ObjectMeta::find(read_attribute(xn, szType, x_string));
        if(!oi)
            return Ref<Rule>();
        Object* obj         = oi->create();
        Ref<Rule>     auth    = dynamic_cast<Rule*>(obj);
        if(auth && auth->load(xn))
            return auth;
        if(obj)
            delete obj;
        return Ref<Rule>();
    }
    
    void    write_xn(XmlNode& xn, const Ref<Rule>&v)
    {
        if(v.valid()){
            write_attribute(xn, szType, v->metaInfo().name());
            v->save(xn);
        }
    }


}


