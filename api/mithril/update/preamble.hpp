////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/basic/Flags.hpp>
#include <0/math/Counter.hpp>

#include <mithril/field/Field.hpp>
#include <mithril/id/Id.hpp>
#include <mithril/update/dependency.hpp>
#include <mithril/update/AllLocal.hpp>

// Use to set echo steps

/*
    A rather annoying macro, if defined, set the logger 
    output to log out *EVERY* call to an update routine
*/
#if !defined(NDEBUG)
    //#define YQ_UPDATE_ECHO_STEPS        yNotice()
#endif

namespace yq::mithril {
    struct Class;
    struct Field;
    struct FileSpec;
    struct Tag;
}

namespace yq::mithril::update {
    using HopCount              = HCountI8;
    using hop_t                 = int8_t;

    using ClassHopMap           = std::map<Class,HopCount>;
    using FieldHopMap           = std::map<Field,HopCount>;

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
    
    class UAtom;
    class UCategory;
    class UClass;
    class UDirectory;
    class UDocument;
    class UField;
    class UFolder;
    class UFragment;
    class UImage;
    class ULeaf;
    class UTag;
    class UUser;
    
    ////////////////////////////////////////////////////////////////////////////////////
    //  OLD UPDATE BELOW

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

