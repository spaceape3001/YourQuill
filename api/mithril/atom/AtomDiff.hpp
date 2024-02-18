////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <mithril/atom/Atom.hpp>
//#include <mithril/attribute/Attribute.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/util/Changed.hpp>

namespace yq::mithril {
    struct Atom::Diff {
    
        const Atom                  x;
        const Change                chg;
        const std::string           key;
        
        Changed<Image>              icon;
        Changed<TagSet>             tags;
        Changed<ClassSet>           classes;
        //AttributeDiffSpan           diffs;  //< Will be empty for STARTUP
    };
}
