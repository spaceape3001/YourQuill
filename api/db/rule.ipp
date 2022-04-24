////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "rule.hpp"

namespace yq {
    Rule&       Rule::merge(const Rule&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        actions += b.actions;
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
        ret.name        = read_child(xn, szName, x_sstring);
        ret.actions     = read_children(xn, szAction, x_action);
        ret.triggers    = read_children(xn, szTrigger, x_trigger);
        return true;
    }
    
    void        write_xn(XmlNode*xn, const Rule& v)
    {
        write_child(xn, szName, v.name);
        write_children(xn, szAction, v.actions);
        write_children(xn, szTrigger, v.triggers);
    }
}
