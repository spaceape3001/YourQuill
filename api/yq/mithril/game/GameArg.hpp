////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <yq/mithril/preamble.hpp>

namespace yq::mithril::arg {
    //! Finds game, tries key first, then by ID
    Game game(std::string_view arg_string);
    Game game(const WebContext&, bool *detected=nullptr);
    Game game(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Game game(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

    //! Finds game by ID only
    Game game_id(std::string_view arg_string);
    Game game_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Game game_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    
    //! Finds game by KEY only
    Game game_key(std::string_view arg_string);
    Game game_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Game game_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}

