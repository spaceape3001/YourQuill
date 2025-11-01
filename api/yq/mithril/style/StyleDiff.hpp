////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/enum/Change.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/style/Style.hpp>
#include <yq/mithril/util/Changed.hpp>

namespace yq::mithril {
    struct Style::Diff {
        const Style             x;
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
