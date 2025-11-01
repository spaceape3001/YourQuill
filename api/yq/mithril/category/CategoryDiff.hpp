////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/category/Category.hpp>
#include <yq/mithril/enum/Change.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/util/Changed.hpp>

namespace yq::mithril {
    struct Category::Diff {
        const Category          x;
        const uint64_t          id;
        const Change            chg;
        const std::string       key;
        
        Changed<Image>          icon;
        Changed<std::string>    name;
        Changed<std::string>    brief;
        std::string             notes;  //!< Current notes data
        
        constexpr operator bool() const noexcept 
        {
            return icon || name || brief;
        }
    };
}
