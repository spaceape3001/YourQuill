////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

#include <mithril/enum/SizeDesc.hpp>
#include <mithril/preamble.hpp>

namespace yq::mithril::arg {
    //! Finds value by ID
    Value value(std::string_view arg_string);
    Value value(const WebContext&, bool *detected=nullptr);
    Value value(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Value value(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}
