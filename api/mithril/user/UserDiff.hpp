////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/enum/Change.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/user/User.hpp>
#include <mithril/util/Changed.hpp>

namespace yq::mithril {
    struct User::Diff {
        const User              x;
        const Change            chg;
        const std::string       key;
        
        Changed<Image>          icon;
        Changed<std::string>    name;
        Changed<std::string>    brief;
        Changed<bool>           owner;
        Changed<bool>           admin;
        Changed<bool>           writer;
        Changed<bool>           reader;
        Changed<bool>           guest;

        constexpr operator bool() const noexcept 
        {
            return icon || name || brief || owner || admin || writer || reader || guest;
        }
    };
}
