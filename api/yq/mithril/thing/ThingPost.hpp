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
    Thing thing(WebContext&, bool *detected=nullptr);
    Thing thing(WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Thing thing(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}
