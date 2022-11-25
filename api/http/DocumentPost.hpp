////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <http/Post.hpp>

namespace yq {
    struct WebContext;
    struct Document;
    
    namespace post {
        //! Finds root, by ID
        Document document(WebContext&, bool *detected=nullptr);
        Document document(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Document document(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
