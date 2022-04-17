////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq {
    struct Tag;
    struct WebContext;
    class WebHtml;
    
    namespace arg {
        //! Finds tag, tries key first, then by ID
        Tag tag(std::string_view arg_string);
        Tag tag(const WebContext&);
        Tag tag(const WebHtml&);
        Tag tag(const WebContext&, std::string_view arg_name);
        Tag tag(const WebHtml&, std::string_view arg_name);
        Tag tag(const WebContext&, std::initializer_list<std::string_view> arg_names);
        Tag tag(const WebHtml&, std::initializer_list<std::string_view> arg_names);

        //! Finds tag by ID only
        Tag tag_id(std::string_view arg_string);
        Tag tag_id(const WebContext&, std::string_view arg_name);
        Tag tag_id(const WebHtml&, std::string_view arg_name);
        Tag tag_id(const WebContext&, std::initializer_list<std::string_view> arg_names);
        Tag tag_id(const WebHtml&, std::initializer_list<std::string_view> arg_names);
        
        //! Finds tag by KEY only
        Tag tag_key(std::string_view arg_string);
        Tag tag_key(const WebContext&, std::string_view arg_name);
        Tag tag_key(const WebHtml&, std::string_view arg_name);
        Tag tag_key(const WebContext&, std::initializer_list<std::string_view> arg_names);
        Tag tag_key(const WebHtml&, std::initializer_list<std::string_view> arg_names);
    }
}

