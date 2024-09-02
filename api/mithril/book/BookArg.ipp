////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "BookArg.hpp"


#include <mithril/web/WebContext.hpp>
#include <mithril/book/BookCDB.hpp>

namespace yq::mithril::arg {
    Book book(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Book{};
            
        Book t   = cdb::book( arg_string);
        if(t)
            return t;
        return book_id(arg_string);
    }
    
    Book book(const WebContext&ctx, bool *detected)
    {
        std::string    k    = ctx.find_query("id");
        if(!k.empty())
            return book_id(k);
        
        k       = ctx.find_query("key");
        if(!k.empty())
            return book_key(k);
        
        k       = ctx.find_query("book");
        if(!k.empty())
            return book(k);
        return Book{};
    }
    
    
    Book book(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return book(arg_string);
    }
    
    Book book(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return book(arg_string);
    }

    Book book_id(std::string_view arg_string)
    {
        auto vv = to_uint64(arg_string);
        if(!vv)
            return Book{};
        if(!cdb::exists_book(*vv))
            return Book{};
        return Book{*vv};
    }

    Book book_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return book_id(arg_string);
    }
    
    Book book_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return book_id(arg_string);
    }
    
    Book book_key(std::string_view arg_string)
    {
        return cdb::book(trimmed(arg_string));
    }
    
    Book book_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return book_key(arg_string);
    }
    
    Book book_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return book_key(arg_string);
    }
}
