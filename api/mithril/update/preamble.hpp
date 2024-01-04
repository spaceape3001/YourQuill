////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/basic/Flags.hpp>
#include <0/math/Counter.hpp>
#include <mithril/update/AllLocal.hpp>
#include <mithril/id/Id.hpp>

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
    
    struct UAtom;
    struct UClass;
    struct UField;
    struct UImage;
    struct ULeaf;
    struct UTag;
    

}

