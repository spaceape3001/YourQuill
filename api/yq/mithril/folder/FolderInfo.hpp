////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/folder/Folder.hpp>
#include <mithril/image/Image.hpp>
#include <vector>


namespace yq::mithril {
    struct Folder::Info {
        std::string     brief;
        bool            hidden      = false;
        std::string     key;
        std::string     name;
        Folder          parent;
        bool            removed     = false;
        std::string     skey;
        Image           icon;

        bool operator==(const Info&) const = default;
    };

}
