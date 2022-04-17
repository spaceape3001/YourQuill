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
    class WebHtml;
    struct Root;
    struct Document;
    
    namespace arg {

        //! Finds document, tries key first, then by ID
        Document document(std::string_view arg_string);
        Document document(const WebContext&);
        Document document(const WebHtml&);
        Document document(const WebContext&, std::string_view arg_name);
        Document document(const WebHtml&, std::string_view arg_name);
        Document document(const WebContext&, std::initializer_list<std::string_view> arg_names);
        Document document(const WebHtml&, std::initializer_list<std::string_view> arg_names);

        //! Finds document by ID only
        Document document_id(std::string_view arg_string);
        Document document_id(const WebContext&, std::string_view arg_name);
        Document document_id(const WebHtml&, std::string_view arg_name);
        Document document_id(const WebContext&, std::initializer_list<std::string_view> arg_names);
        Document document_id(const WebHtml&, std::initializer_list<std::string_view> arg_names);
        
        //! Finds document by KEY only
        Document document_key(std::string_view arg_string);
        Document document_key(const WebContext&, std::string_view arg_name);
        Document document_key(const WebHtml&, std::string_view arg_name);
        Document document_key(const WebContext&, std::initializer_list<std::string_view> arg_names);
        Document document_key(const WebHtml&, std::initializer_list<std::string_view> arg_names);

    }
}
