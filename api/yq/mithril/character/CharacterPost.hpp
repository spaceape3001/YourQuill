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
    Character character(WebContext&, bool *detected=nullptr);
    Character character(WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Character character(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}
