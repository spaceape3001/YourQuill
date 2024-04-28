////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/enum/Change.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/drafter/Drafter.hpp>
#include <mithril/util/Changed.hpp>

namespace yq::mithril {
    struct Drafter::Diff {
        const Drafter             x;
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
