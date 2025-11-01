////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DocumentPost.hpp"

#include <mithril/document/DocumentArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/document/Document.hpp>
#include <yq/text/copy.hpp>

namespace yq::mithril::post {
    Document document(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("document");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::document_id(k);
        }
        return Document();
    }
    
    Document document(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::document_id(arg_string);
    }
    
    Document document(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::document_id(arg_string);
    }
}
