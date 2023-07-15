////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <update/preamble.hpp>
#include <mithril/class/ClassData.hpp>

namespace yq::mithril::update {
    struct UClass {
        static UClass&      get(Class c);

        const Class         class_;
        const uint64_t      id;
        const std::string   key;
        Class::SharedData   def;
        
        UClass(Class);

    #if 0    
        StringCountMap  alias;
        ClassCountMap   base;
        ClassCountMap   derive;
        StringCountMap  prefix;
        ClassCountMap   reverse;
        ClassCountMap   source;
        StringCountMap  suffix;
        ClassCountMap   target;
    #endif

    };
}
