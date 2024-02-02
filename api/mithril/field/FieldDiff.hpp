////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/category/Category.hpp>
#include <mithril/class/Class.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/enum/Multiplicity.hpp>
#include <mithril/enum/Restriction.hpp>
#include <mithril/field/Field.hpp>
#include <mithril/id/Id.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/util/Changed.hpp>

namespace yq::mithril {
    struct Field::Diff {
        const Field                 x;
        const Change                chg;
        const std::string           key;
        const std::string           subkey;
        const Class                 cls;
        const bool                  anycls;
        
        Changed<string_set_t>       aliases;
        Changed<ClassHopMap>        atom_types;
        Changed<std::string>        brief;
        Changed<Category>           category;
        Changed<ClassSet>           classes;
        Changed<std::set<uint64_t>> data_types;
        Changed<Id>                 expected;
        Changed<Image>              icon;
        Changed<uint64_t>           max_count;
        Changed<Multiplicity>       multiplicity;
        Changed<std::string>        name;
        Changed<std::string>        pkey;
        Changed<std::string>        plural;
        Changed<Restriction>        restriction;
        Changed<TagSet>             tags;
        Changed<string_set_t>       usurps;
    };
}
