////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Action.hpp"
#include "Constraint.hpp"
#include "Rule.hpp"
#include "Trigger.hpp"
#include <yq/meta/ObjectInfoWriter.hpp>

namespace {
    void    reg_rule_meta() 
    {
        using namespace yq;
        using namespace yq::mithril;

        auto w = writer<Rule>();
        w.label("Generic Rule");
        w.property("name", &Rule::name);
    }
}
