////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <yq/mithril/html/Post.hpp>

namespace yq::mithril {
    struct WebContext;
    struct Document;
    
    namespace post {
        //! Finds root_dir, by ID
        Document document(WebContext&, bool *detected=nullptr);
        Document document(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Document document(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
