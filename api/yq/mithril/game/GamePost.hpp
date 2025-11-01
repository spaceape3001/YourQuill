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
    Game game(WebContext&, bool *detected=nullptr);
    Game game(WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Game game(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}
