////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/preamble.hpp>
#include <yq/mithril/folder/Folder.hpp>
#include <yq/mithril/image/Image.hpp>
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
