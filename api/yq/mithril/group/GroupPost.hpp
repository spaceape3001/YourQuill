////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <yq/mithril/html/Post.hpp>
#include <yq/mithril/preamble.hpp>

namespace yq::mithril::post {
    //! Finds root_dir, by ID
    Group group(WebContext&, bool *detected=nullptr);
    Group group(WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Group group(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}
