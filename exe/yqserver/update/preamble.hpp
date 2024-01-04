////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/update/preamble.hpp>
#include <mithril/update/dependency.hpp>

#include <0/basic/preamble.hpp>
#include <0/basic/Flags.hpp>
#include <0/math/Counter.hpp>
#include <0/meta/TypeInfo.hpp>
#include <mithril/class/Class.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/field/Field.hpp>
#include <mithril/id/Id.hpp>
#include <mithril/update/AllLocal.hpp>
#include <mithril/update/ById.hpp>
#include <mithril/update/U.hpp>

namespace yq::mithril::update {

    using OutClass  = dependency::Outbound<Class>;
    using OutField  = dependency::Outbound<Field>;
    using NodeClass = dependency::Node<Class>;

#if 0

    struct Outbound { 
        Class cls; 
        constexpr bool operator==(const Outbound&) const noexcept = default;
    };
    
    struct Node { 
        Class cls; 
        constexpr bool operator==(const Node&) const noexcept = default;
    };
#endif

    using Resolve       = std::variant<std::monostate,NodeClass,Field,OutClass>;

    //  ResolveMap *might* become multimap
    using ResolveMap    = std::map<std::string,Resolve,IgCase>;
    using ResolveMMap   = std::multimap<std::string,Resolve,IgCase>;
    
}
