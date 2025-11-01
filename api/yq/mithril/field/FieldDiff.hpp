////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/category/Category.hpp>
#include <yq/mithril/class/Class.hpp>
#include <yq/mithril/enum/Change.hpp>
#include <yq/mithril/enum/Multiplicity.hpp>
#include <yq/mithril/enum/Restriction.hpp>
#include <yq/mithril/field/Field.hpp>
#include <yq/mithril/id/Id.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/util/Changed.hpp>

namespace yq::mithril {
    struct Field::Diff {
        const Field                 x;
        const uint64_t              id;
        const Change                chg;
        const std::string           key;
        const std::string           subkey;
        const Class                 cls;
        const bool                  anycls;
        
        Changed<string_set_t>       aliases;
        Changed<std::string>        brief;
        Changed<Category>           category;
        Changed<ClassSet>           classes;
        Changed<Id>                 expected;
        Changed<Image>              icon;
        Changed<uint64_t>           max_count;
        Changed<Multiplicity>       multiplicity;
        Changed<std::string>        name;
        Changed<std::string>        pkey;
        Changed<std::string>        plural;
        Changed<Restriction>        restriction;
        Changed<TagSet>             tags;
        Changed<std::set<Id>>       types;
        Changed<string_set_t>       usurps;
    };
}
