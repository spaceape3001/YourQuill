////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/xml/XmlObject.hpp>
#include <yq/core/Ref.hpp>

namespace yq::mithril {
    //  Still a work in progress....
    //  might go into elements....
    class Rule : public XmlObject, public RefCount {
        YQ_OBJECT_DECLARE(Rule, XmlObject)
    public:

        std::string             name;

        Rule();
        virtual ~Rule();

        // 
        virtual void something_todo() {}

        static void init_meta();


        //Vector<Ref<Action>>     actions;
        //Vector<Ref<Constraint>> constraints;
        //Vector<Ref<Trigger>>    triggers;
        //std::string             name;
        
        //Rule&       merge(const Rule&, bool fOverride);
    };
    
    Ref<Rule>   x_rule(const XmlNode&);
    //bool        read_xn(Rule&, const XmlNode&);
    void        write_xn(XmlNode&, const Ref<Rule>&);
    
}

