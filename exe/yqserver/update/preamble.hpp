////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

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
#include <unordered_map>

// Use to set echo steps

/*
    A rather annoying macro, if defined, set the logger 
    output to log out *EVERY* call to an update routine
*/
#if !defined(NDEBUG)
    //#define YQ_UPDATE_ECHO_STEPS        yNotice()
#endif

namespace yq::mithril {
    struct Field;
    struct FileSpec;
}

namespace yq::mithril::update {

    using HopCount              = HCountI8;
    using hop_t                 = int8_t;

    using ClassHopMap           = std::map<Class,HopCount>;
    using FieldHopMap           = std::map<Field,HopCount>;

    struct UClass;
    struct UField;
    
    enum class F {
        LOCK,
        NOTIFY,
        
        
        LOAD,
        INFO,
        ICON,
        TAGS     
    };
    
    using FF        = Flags<F>;
    
    template <typename T>
    using AL = AllLocal<T>;

    template <IdType T>
    struct DD {
        std::map<T,HopCount>    hop;
        std::set<T>             all;        //  All (for fields)
        std::set<T>             direct;
    };
    

    struct Outbound { 
        Class cls; 
        constexpr bool operator==(const Outbound&) const noexcept = default;
    };
    
    struct Node { 
        Class cls; 
        constexpr bool operator==(const Node&) const noexcept = default;
    };

    using Resolve       = std::variant<std::monostate,Node,Field,Outbound>;

    //  ResolveMap *might* become multimap
    using ResolveMap    = std::map<std::string,Resolve,IgCase>;
    using ResolveMMap   = std::multimap<std::string,Resolve,IgCase>;
    
}
