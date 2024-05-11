////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#error "Do NOT include (for now)"

#include <mithril/enum/Change.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/doodle/Doodle.hpp>
#include <mithril/util/Changed.hpp>

namespace yq::mithril {
    struct Doodle::Diff {
        const Doodle             x;
        const uint64_t          id;
        const Change            chg;
        const std::string       key;
        
        Changed<Image>          icon;
        Changed<std::string>    name;
        Changed<std::string>    brief;
        
        constexpr operator bool() const noexcept 
        {
            return icon || name || brief;
        }
        
    };
}
