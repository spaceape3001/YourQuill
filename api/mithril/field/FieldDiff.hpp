////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/category/Category.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/enum/Multiplicity.hpp>
#include <mithril/enum/Restriction.hpp>
#include <mithril/id/Id.hpp>
#include <mithril/field/Field.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/util/Changed.hpp>

namespace yq::mithril {
    struct Field::Diff {
        const Field                 x;
        const Change                chg;
        const std::string           key;
        
        Changed<Image>              icon;
        Changed<std::string>        name;
        Changed<std::string>        pkey;
        Changed<std::string>        brief;
        Changed<Category>           category;
        Changed<Id>                 expected;
        Changed<Multiplicity>       multiplicity;
        Changed<Restriction>        restriction;
        Changed<uint64_t>           max_count;
    };
}
