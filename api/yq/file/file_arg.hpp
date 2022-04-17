////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Directory.hpp"
#include "Document.hpp"
#include "Folder.hpp"
#include "Fragment.hpp"

#include <initializer_list>
#include <string_view>

namespace yq {
    struct WebContext;
    class WebHtml;
    struct Root;
    
    namespace arg {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //! Finds tag, tries key first, then by ID
        Directory directory(std::string_view arg_string, const Root* rt=nullptr);
        Directory directory(const WebContext&, const Root* rt=nullptr);
        Directory directory(const WebHtml&, const Root* rt=nullptr);
        Directory directory(const WebContext&, std::string_view arg_name, const Root* rt=nullptr);
        Directory directory(const WebHtml&, std::string_view arg_name, const Root* rt=nullptr);
        Directory directory(const WebContext&, std::initializer_list<std::string_view> arg_names, const Root* rt=nullptr);
        Directory directory(const WebHtml&, std::initializer_list<std::string_view> arg_names, const Root* rt=nullptr);

        //! Finds directory by ID only
        Directory directory_id(std::string_view arg_string);
        Directory directory_id(const WebContext&, std::string_view arg_name);
        Directory directory_id(const WebHtml&, std::string_view arg_name);
        Directory directory_id(const WebContext&, std::initializer_list<std::string_view> arg_names);
        Directory directory_id(const WebHtml&, std::initializer_list<std::string_view> arg_names);
        
        //! Finds directory by KEY only
        Directory directory_key(std::string_view arg_string, const Root* rt=nullptr);
        Directory directory_key(const WebContext&, std::string_view arg_name, const Root* rt=nullptr);
        Directory directory_key(const WebHtml&, std::string_view arg_name, const Root* rt=nullptr);
        Directory directory_key(const WebContext&, std::initializer_list<std::string_view> arg_names, const Root* rt=nullptr);
        Directory directory_key(const WebHtml&, std::initializer_list<std::string_view> arg_names, const Root* rt=nullptr);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        //! Finds folder, tries key first, then by ID
        Folder folder(std::string_view arg_string);
        Folder folder(const WebContext&);
        Folder folder(const WebHtml&);
        Folder folder(const WebContext&, std::string_view arg_name);
        Folder folder(const WebHtml&, std::string_view arg_name);
        Folder folder(const WebContext&, std::initializer_list<std::string_view> arg_names);
        Folder folder(const WebHtml&, std::initializer_list<std::string_view> arg_names);

        //! Finds folder by ID only
        Folder folder_id(std::string_view arg_string);
        Folder folder_id(const WebContext&, std::string_view arg_name);
        Folder folder_id(const WebHtml&, std::string_view arg_name);
        Folder folder_id(const WebContext&, std::initializer_list<std::string_view> arg_names);
        Folder folder_id(const WebHtml&, std::initializer_list<std::string_view> arg_names);
        
        //! Finds folder by KEY only
        Folder folder_key(std::string_view arg_string);
        Folder folder_key(const WebContext&, std::string_view arg_name);
        Folder folder_key(const WebHtml&, std::string_view arg_name);
        Folder folder_key(const WebContext&, std::initializer_list<std::string_view> arg_names);
        Folder folder_key(const WebHtml&, std::initializer_list<std::string_view> arg_names);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        //! Finds fragment, tries key first, then by ID
        Fragment fragment(std::string_view arg_string, const Root* rt=nullptr);
        Fragment fragment(const WebContext&, const Root* rt=nullptr);
        Fragment fragment(const WebHtml&, const Root* rt=nullptr);
        Fragment fragment(const WebContext&, std::string_view arg_name, const Root* rt=nullptr);
        Fragment fragment(const WebHtml&, std::string_view arg_name, const Root* rt=nullptr);
        Fragment fragment(const WebContext&, std::initializer_list<std::string_view> arg_names, const Root* rt=nullptr);
        Fragment fragment(const WebHtml&, std::initializer_list<std::string_view> arg_names, const Root* rt=nullptr);

        //! Finds fragment by ID only
        Fragment fragment_id(std::string_view arg_string);
        Fragment fragment_id(const WebContext&, std::string_view arg_name);
        Fragment fragment_id(const WebHtml&, std::string_view arg_name);
        Fragment fragment_id(const WebContext&, std::initializer_list<std::string_view> arg_names);
        Fragment fragment_id(const WebHtml&, std::initializer_list<std::string_view> arg_names);
        
        //! Finds fragment by KEY only
        Fragment fragment_key(std::string_view arg_string, const Root* rt=nullptr);
        Fragment fragment_key(const WebContext&, std::string_view arg_name, const Root* rt=nullptr);
        Fragment fragment_key(const WebHtml&, std::string_view arg_name, const Root* rt=nullptr);
        Fragment fragment_key(const WebContext&, std::initializer_list<std::string_view> arg_names, const Root* rt=nullptr);
        Fragment fragment_key(const WebHtml&, std::initializer_list<std::string_view> arg_names, const Root* rt=nullptr);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //! Finds root, tries key first, then by ID
        const Root* root(std::string_view arg_string);
        const Root* root(const WebContext&);
        const Root* root(const WebHtml&);
        const Root* root(const WebContext&, std::string_view arg_name);
        const Root* root(const WebHtml&, std::string_view arg_name);
        const Root* root(const WebContext&, std::initializer_list<std::string_view> arg_names);
        const Root* root(const WebHtml&, std::initializer_list<std::string_view> arg_names);

        //! Finds root by ID only
        const Root* root_id(std::string_view arg_string);
        const Root* root_id(const WebContext&, std::string_view arg_name);
        const Root* root_id(const WebHtml&, std::string_view arg_name);
        const Root* root_id(const WebContext&, std::initializer_list<std::string_view> arg_names);
        const Root* root_id(const WebHtml&, std::initializer_list<std::string_view> arg_names);
        
        //! Finds root by key only
        const Root* root_key(std::string_view arg_string);
        const Root* root_key(const WebContext&, std::string_view arg_name);
        const Root* root_key(const WebHtml&, std::string_view arg_name);
        const Root* root_key(const WebContext&, std::initializer_list<std::string_view> arg_names);
        const Root* root_key(const WebHtml&, std::initializer_list<std::string_view> arg_names);
    }
}
