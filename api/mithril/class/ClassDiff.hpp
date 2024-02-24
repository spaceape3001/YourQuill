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

        Changed<ClassSet>       uses;
        Changed<TagSet>         tags;
    };
}
