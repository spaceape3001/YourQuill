////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <mithril/html/Post.hpp>
#include <mithril/preamble.hpp>

namespace yq::mithril::post {
    //! Finds root_dir, by ID
    Event event(WebContext&, bool *detected=nullptr);
    Event event(WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Event event(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}
