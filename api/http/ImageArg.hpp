////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

#include <kernel/SizeDesc.hpp>

namespace yq {
    struct Image;
    struct WebContext;

    namespace arg {
        //! Finds image by ID
        Image image(std::string_view arg_string);
        Image image(const WebContext&, bool *detected=nullptr);
        Image image(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Image image(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    
        SizeDesc    size_desc(std::string_view);
        SizeDesc    size_desc(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        SizeDesc    size_desc(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
