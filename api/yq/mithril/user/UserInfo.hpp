////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/preamble.hpp>
#include <yq/mithril/document/Document.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/user/UserFile.hpp>

namespace yq::mithril {        
    struct User::Info {
        Document        doc;
        std::string     key;
        Image           icon;
        std::string     name;
        std::string     brief;
        bool            is_owner    = false;
        bool            is_admin    = false;
        bool            is_writer   = false;
        bool            is_reader   = false;
        bool            is_guest   = false;
        bool operator==(const Info&) const = default;
    };

    using UserFragDoc  = std::pair<Fragment, User::SharedFile>;
}
