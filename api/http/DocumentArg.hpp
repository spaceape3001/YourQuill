////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq {
    struct WebContext;
    struct Root;
    struct Document;
    
    namespace arg {

        //! Finds document, tries key first, then by ID
        Document document(std::string_view arg_string);
        Document document(const WebContext&, bool *detected=nullptr);
        Document document(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Document document(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds document by ID only
        Document document_id(std::string_view arg_string);
        Document document_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Document document_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds document by KEY only
        Document document_key(std::string_view arg_string);
        Document document_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Document document_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

    }

    namespace post {
        //! Finds root, by ID
        Document document(WebContext&, bool *detected=nullptr);
        Document document(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Document document(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
