////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/enum/Change.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/leaf/Leaf.hpp>
#include <mithril/tag/Tag.hpp>
#include <mithril/util/Changed.hpp>

namespace yq::mithril {
    struct Tag::Diff {
        const Tag               x;
        const Change            chg;
        const std::string       key;
        
        Changed<Leaf>           leaf;
        Changed<Image>          icon;
        Changed<std::string>    name;
        Changed<std::string>    brief;
        
        constexpr operator bool() const noexcept 
        {
            return leaf || icon || name || brief;
        }
        
    };
}
