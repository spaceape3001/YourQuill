////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <mithril/preamble.hpp>

namespace yq::mithril::arg {
    //! Finds book, tries key first, then by ID
    Book book(std::string_view arg_string);
    Book book(const WebContext&, bool *detected=nullptr);
    Book book(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Book book(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

    //! Finds book by ID only
    Book book_id(std::string_view arg_string);
    Book book_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Book book_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    
    //! Finds book by KEY only
    Book book_key(std::string_view arg_string);
    Book book_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Book book_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}

