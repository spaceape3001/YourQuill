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
    Value value(WebContext&, bool *detected=nullptr);
    Value value(WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Value value(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}
