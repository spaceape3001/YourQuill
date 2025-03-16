////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "BookPost.hpp"

#include <mithril/book/BookArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/book/Book.hpp>

#include <yq/text/copy.hpp>

namespace yq::mithril::post {
    Book book(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("book");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::book_id(k);
        }
        return Book();
    }
    
    Book book(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::book_id(arg_string);
    }
    
    Book book(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::book_id(arg_string);
    }
}
