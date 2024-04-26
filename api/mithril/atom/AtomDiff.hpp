////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <mithril/atom/Atom.hpp>
#include <mithril/attribute/Attribute.hpp>
#include <mithril/attribute/AttributeDiff.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/util/Changed.hpp>

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
        Changed<std::string>        title;
        Changed<std::string>        abbreviation;
        
        std::span<Attribute::Diff>  diffs;  //< Will be empty for STARTUP
    };
}
