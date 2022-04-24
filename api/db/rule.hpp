////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "action.hpp"
#include "trigger.hpp"

namespace yq {
    struct Rule {
        Vector<Ref<Action>>     actions;
        Vector<Ref<Trigger>>    triggers;
        std::string             name;
        
        Rule&       merge(const Rule&, bool fOverride);
    };
    
    Rule        x_rule(const XmlNode*);
    bool        read_xn(Rule&, const XmlNode*);
    void        write_xn(XmlNode*, const Rule&);
    
}

