////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/category/Category.hpp>
#include <mithril/class/Class.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/util/Changed.hpp>

namespace yq::mithril {
    struct Class::Diff {
        const Class             x;
        const uint64_t          id;
        const Change            chg;
        const std::string       key;
        
        Changed<Category>       category;
        Changed<Image>          icon;
        Changed<std::string>    name;
        Changed<std::string>    plural;
        Changed<std::string>    brief;

        //! Aliases as DEFINED in the file
        Changed<string_set_t>   aliases;
        
        //! Prefixes as DEFINED in the file
        Changed<string_set_t>   prefixes;

        //! Reverses as DEFINED in the file
        Changed<ClassSet>       reverses;
        
        //! Sources as DEFINED in the file
        Changed<ClassSet>       sources;
        
        //! Suffixes as DEFINED in the file
        Changed<string_set_t>   suffixes;
        
        //! TAGS (these aren't propagated)
        Changed<TagSet>         tags;
        
        //! Targets as DEFINED in the file
        Changed<ClassSet>       targets;
        
        //! Uses as DEFINED in the file
        Changed<ClassSet>       uses;

        //Changed<string_set_t>   lookups;
        Changed<ClassHopMap>    bases;
        Changed<ClassHopMap>    deriveds;
    };
}
