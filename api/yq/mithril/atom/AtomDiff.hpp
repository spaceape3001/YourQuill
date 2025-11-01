////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <yq/mithril/atom/Atom.hpp>
#include <yq/mithril/attribute/Attribute.hpp>
#include <yq/mithril/attribute/AttributeDiff.hpp>
#include <yq/mithril/enum/Change.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/util/Changed.hpp>

namespace yq::mithril {
    struct Atom::Diff {
    
        const Atom                  x;
        const uint64_t              id;
        const Document              doc;
        const Change                chg;
        const std::string           key;
        
        Changed<Image>              icon;
        Changed<TagSet>             tags;
        Changed<ClassSet>           classes;
        Changed<FieldSet>           fields;
        Changed<AttributeSet>       attributes;
        Changed<std::string>        name;
        Changed<std::string>        brief;
        Changed<std::string>        abbreviation;
        
        std::span<const Attribute::Diff>  diffs;
    };
}
